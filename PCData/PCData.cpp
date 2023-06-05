//
// Created by Kenneth Balslev on 03/06/2023.
//

#include "PCData.hpp"

#include <nlohmann/json.hpp>
#include <pugixml.hpp>

#include <etl/flat_map.h>
#include <set>

namespace
{

    std::string toupper(std::string str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
        return str;
    }

    auto findCompoundByName(pugi::xml_node compounds, const std::string& name)
    {
        return compounds.find_child([&name](pugi::xml_node node) {
            auto compoundName = std::string(node.child("CompoundID").attribute("value").as_string());
            compoundName      = toupper(compoundName);
            return compoundName == name;
        });
    }

    const auto identifierNames = etl::flat_map<std::string, std::string, 15> { { "CompoundID", "Name" }, { "CAS", "CAS" } };

    const auto constantNames =
        etl::flat_map<std::string, std::string, 15> { { "CriticalTemperature", "CriticalTemperature" },
                                                      { "CriticalPressure", "CriticalPressure" },
                                                      { "CriticalVolume", "CriticalVolume" },
                                                      { "CriticalCompressibility", "CriticalCompressibility" },
                                                      { "NormalBoilingPointTemperature", "NormalBoilingPointTemperature" },
                                                      { "TriplePointTemperature", "TriplePointTemperature" },
                                                      { "TriplePointPressure", "TriplePointPressure" },
                                                      { "MolecularWeight", "MolecularWeight" },
                                                      { "AcentricityFactor", "AcentricFactor" },
                                                      { "HeatOfFormation", "IGEnthalpyOfFormation" },
                                                      { "GibbsEnergyOfFormation", "IGGibbsEnergyOfFormation" },
                                                      { "AbsEntropy", "IGEntropy" },
                                                      { "HeatOfCombustion", "EnthalpyOfCombustion" } };

    const auto correlationNames = etl::flat_map<std::string, std::string, 11> {
        { "LiquidDensity", "LiquidDensity" },
        { "VaporPressure", "VaporPressure" },
        { "AntoineVaporPressure", "VaporPressure" },
        { "HeatOfVaporization", "HeatOfVaporization" },
        { "LiquidHeatCapacityCp", "LiquidHeatCapacityCp" },
        { "IdealGasHeatCapacityCp", "IdealGasHeatCapacityCp" },
        { "LiquidViscosity", "LiquidViscosity" },
        { "VaporViscosity", "VaporViscosity" },
        { "LiquidThermalConductivity", "LiquidThermalConductivity" },
        { "VaporThermalConductivity", "VaporThermalConductivity" },
        { "SurfaceTension", "SurfaceTension" },
    };

    void extractCompoundData(pugi::xml_node compound, nlohmann::json& result)
    {
        using json = nlohmann::json;

        // Extract the data from the xml file
        for (auto& node : compound.children()) {
            // Extract the identifiers from the xml data
            auto id = identifierNames.find(node.name());
            if (id != identifierNames.end()) {
                result[id->second] = toupper(node.attribute("value").as_string());
                continue;
            }

            // Extract the pure component constants from the xml data
            auto constant = constantNames.find(node.name());
            if (constant != constantNames.end()) {
                result[constant->second]["Value"] = node.attribute("value").as_double();
                result[constant->second]["Unit"]  = node.attribute("units").as_string();
                continue;
            }

            // Extract the temperature dependent correlations from the xml data
            auto corr = correlationNames.find(node.name());
            if (corr != correlationNames.end()) {
                json correlation {};
                correlation["Unit"] = node.attribute("units").as_string();
                for (const auto& child : node.children()) {
                    if (std::string(child.name()) == "eqno")
                        correlation[child.name()] = child.attribute("value").as_int();
                    else if (std::string(child.name()) == "Tmin" || std::string(child.name()) == "Tmax") {
                        correlation[child.name()]["Value"] = child.attribute("value").as_double();
                        correlation[child.name()]["Unit"]  = child.attribute("units").as_string();
                    }

                    else
                        correlation[child.name()] = child.attribute("value").as_double();
                }
                result[corr->second].emplace_back(correlation);
                continue;
            }
        }
    }
}    // namespace

struct PCData::Impl
{
    std::string        m_dbPath;
    pugi::xml_document m_xmldoc;
};

PCData::PCData(const fs::path& dbPath) : m_impl(std::make_unique<Impl>())
{
    m_impl->m_dbPath = dbPath.filename().string();
    m_impl->m_xmldoc.load_file(m_impl->m_dbPath.c_str());
}

PCData::PCData(PCData&& other) noexcept = default;

PCData::~PCData() = default;

PCData& PCData::operator=(PCData&& other) noexcept = default;

std::vector<std::string> PCData::names() const
{
    std::set<std::string> uniqueNames;
    auto                  compounds = m_impl->m_xmldoc.first_child();

    for (const auto& node : compounds.children()) {
        for (const auto& prop : node.children()) {
            uniqueNames.insert(prop.name());
        }
    }

    std::vector<std::string> names { uniqueNames.begin(), uniqueNames.end() };
    return names;
}

JSONString PCData::find(std::string key) const
{
    nlohmann::json result {};

    key            = toupper(key);
    auto compounds = m_impl->m_xmldoc.first_child();
    auto compound  = findCompoundByName(compounds, key);
    if (compound) extractCompoundData(compound, result);

    return result.dump(4);
}

void PCData::erase(std::string key)
{
    key            = toupper(key);
    auto compounds = m_impl->m_xmldoc.first_child();
    auto compound  = findCompoundByName(compounds, key);
    if (compound) compounds.remove_child(compound);
}

void PCData::commit() { m_impl->m_xmldoc.save_file(m_impl->m_dbPath.c_str()); }
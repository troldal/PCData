//
// Created by Kenneth Balslev on 03/06/2023.
//

#ifndef PCDATA_MAIN_PCDATA_HPP
#define PCDATA_MAIN_PCDATA_HPP

#include <memory>
#include <string>
#include <vector>
#include <filesystem>

using JSONString = std::string;
namespace fs = std::filesystem;

class PCData
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;

public:
    PCData(const fs::path& dbPath);

    PCData(const PCData& other) = delete;

    PCData(PCData&& other) noexcept;

    ~PCData();

    PCData& operator=(const PCData& other) = delete;

    PCData& operator=(PCData&& other) noexcept;

    std::vector<std::string> names() const;

    JSONString find(std::string key) const;

    void erase(std::string key);

    void insert(std::string key, const JSONString& value);

    void update(std::string key, const JSONString& value);

    void replace(std::string key, const JSONString& value);

    void commit();
};

#endif    // PCDATA_MAIN_PCDATA_HPP

#pragma once

#include <list>
#include <string>
#include <algorithm>
#include <optional>
#include <iostream>

class Options
{
public:
    Options(int argc, char *argv[])
    {
        for (int i = 1; i < argc; i++)
        {
            mOptList.emplace_back(std::string(argv[i]));
        }
    }

    template <typename T>
    void getOptValue(std::string option, std::optional<T> &val)
    {
        val = getOptValue(option);
    }

    void getOptValue(std::string option, std::optional<unsigned> &val)
    {
        auto opt = getOptValue(option);
        if (!opt.has_value())
        {
            val = {};
            return;
        }

        try
        {
            val = std::stoul(opt.value());
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << " " << opt.value() << std::endl;
            val = {};
        }
    }

private:
    std::list<std::string> mOptList;
    auto hasOption(std::string option) const
    {
        auto it = std::find(mOptList.cbegin(), mOptList.cend(), option);
        return it;
    }

    template <typename T>
    bool optValid(T it) const
    {
        return it != mOptList.cend();
    }

    std::optional<std::string> getOptValue(std::string option)
    {
        auto it = hasOption(option);
        if (!optValid(it))
        {
            // return option not available
            return {};
        }
        if (!optValid(std::next(it)))
        {
            // option incomplete
            return {};
        }
        return *std::next(it);
    }
};
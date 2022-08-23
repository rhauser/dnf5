/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "utils/bgettext/bgettext-mark-domain.h"

#include <fmt/format.h>
#include <libdnf/base/base.hpp>
#include <sys/wait.h>
#include <unistd.h>
#include <rpm/rpmmacro.h>

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace libdnf::plugin;

namespace {

constexpr const char * PLUGIN_NAME = "dbpath";
constexpr PluginVersion PLUGIN_VERSION{0, 1, 0};

constexpr const char * attrs[]{"author.name", "author.email", "description", nullptr};
constexpr const char * attrs_value[]{"Reiner Hauser", "reiner.hauser@cern.ch", "DBPath Plugin."};

class DBPath: public IPlugin {
public:
    DBPath(libdnf::Base & base, libdnf::ConfigParser &) : base(base) {}
    virtual ~DBPath() = default;

    APIVersion get_api_version() const noexcept override { return PLUGIN_API_VERSION; }

    const char * get_name() const noexcept override { return PLUGIN_NAME; }

    PluginVersion get_version() const noexcept override { return PLUGIN_VERSION; }

    const char * const * get_attributes() const noexcept override { return attrs; }

    const char * get_attribute(const char * attribute) const noexcept override {
        for (size_t i = 0; attrs[i]; ++i) {
            if (std::strcmp(attribute, attrs[i]) == 0) {
                return attrs_value[i];
            }
        }
        return nullptr;
    }

    void post_base_setup() override;



private:
    libdnf::Base & base;
};

void DBPath::post_base_setup()
{
    auto dbpath = base.get_config().dbpath().get_value();
    if(!dbpath.empty()) {
        rpmDefineMacro(nullptr, ("_dbpath " + dbpath).c_str(), 0);
    }
}


}  // namespace

APIVersion libdnf_plugin_get_api_version(void) {
    return PLUGIN_API_VERSION;
}

const char * libdnf_plugin_get_name(void) {
    return PLUGIN_NAME;
}

PluginVersion libdnf_plugin_get_version(void) {
    return PLUGIN_VERSION;
}

IPlugin * libdnf_plugin_new_instance(
    [[maybe_unused]] APIVersion api_version, libdnf::Base & base, libdnf::ConfigParser & parser) try {
    return new DBPath(base, parser);
} catch (...) {
    return nullptr;
}

void libdnf_plugin_delete_instance(IPlugin * plugin_object) {
    delete plugin_object;
}

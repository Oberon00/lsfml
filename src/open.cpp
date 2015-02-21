// Part of the LSFML library -- Copyright (c) Christian Neumüller 2015
// This file is subject to the terms of the BSD 2-Clause License.
// See LICENSE.txt or http://opensource.org/licenses/BSD-2-Clause

#include "open_util.hpp"
#include <SFML/Config.hpp>
#include <apollo/builtin_types.hpp>
#include <apollo/create_table.hpp>

LSFML_MODULE(lsfml)
{
    apollo::new_table(L)
        ("version", "0.0.0")
        ("sfml_version",
            BOOST_STRINGIZE(SFML_VERSION_MAJOR)
            "." BOOST_STRINGIZE(SFML_VERSION_MINOR)
            "." BOOST_STRINGIZE(SFML_VERSION_PATCH));

    return 1;
}

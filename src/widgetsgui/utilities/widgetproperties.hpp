/**
 * Addle source code
 * @file
 * @copyright Copyright 2020 Eleanor Hawk
 * @copyright Modification and distribution permitted under the terms of the
 * MIT License. See "LICENSE" for full details.
 */

#ifndef WIDGETPROPERTIES_HPP
#define WIDGETPROPERTIES_HPP

namespace Addle {

// Names of properties of QWidgets and other objects relevant to the gui, for
// convenience / consistencty in property bindings. Not exhaustive, may be
// expanded as desired.
namespace WidgetProperties
{
    //common
    const char* const enabled = "enabled";
    const char* const visible = "visible";

    const char* const icon = "icon";
    const char* const cursor = "cursor";
    const char* const text = "text";
    const char* const toolTip = "toolTip";

    const char* const value = "value";

    // sliders
    const char* const minimum = "minimum";
    const char* const maximum = "maximum";
    const char* const pageStep = "pageStep";
    const char* const singleStep = "singleStep";
}

} // namespace Addle

#endif // WIDGETPROPERTIES_HPP

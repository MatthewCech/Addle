#ifndef IERRORPRESENTER_HPP
#define IERRORPRESENTER_HPP

#include <QString>

#include "interfaces/traits.hpp"


class IErrorPresenter 
{
public:

    enum Severity
    {
        info,
        warning,
        critial
    };

    virtual ~IErrorPresenter() = default;

    virtual void initialize(QString message = QString(), Severity severity = Severity::warning) = 0;

    virtual Severity getSeverity() = 0;
    virtual void setSeverity(Severity severity) = 0;

    virtual QString getMessage() = 0;
    virtual void setMessage(QString message) = 0;
};

DECL_MAKEABLE(IErrorPresenter)
DECL_EXPECTS_INITIALIZE(IErrorPresenter)

#endif // IERRORPRESENTER_HPP
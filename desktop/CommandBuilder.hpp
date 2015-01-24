#ifndef COMMANDBUILDER_HPP
#define COMMANDBUILDER_HPP

#include <QObject>
#include "roboclient.h"

class CommandBuilder
{
public:
    CommandBuilder();

    quint8 getCommand() const;
    void reset();

    void appendFlag( int key );
    void removeFlag( int key );

private:
    quint8 mCommand;
};

#endif // COMMANDBUILDER_HPP

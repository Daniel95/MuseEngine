#pragma once

namespace Muse
{
    struct SystemID
    {
    protected:
        static unsigned short RegisterID()
        {
            static unsigned short lastRegisteredID = 0;
            unsigned short returnID = lastRegisteredID;
            lastRegisteredID++;

            return returnID;
        }
    };

    template <typename T>
    struct SystemRegistry : public SystemID
    {
        static unsigned short ID()
        {
            static const unsigned short id = RegisterID();
            return id;
        }
    };
}

//
// Created by brachwal on 08.04.2020.
//

#ifndef CONFIGSVC_UNITSTATE_HH
#define CONFIGSVC_UNITSTATE_HH

class ConfigModule;

////////////////////////////////////////////////////////////////////////////////
///
class UnitState {
    private:
        ///
        bool m_is_initialized = false;

        ///
        bool m_is_default_value = true;

        ///
        bool m_is_public = true;

        ///
        void IsInitialized(bool val) { m_is_initialized = val; }

        ///
        void IsDefaultValue(bool val) { m_is_default_value = val; }

        ///
        void IsPublic(bool val) { m_is_public = val; }

        ///
        friend class ConfigModule;

    public:
        ///
        UnitState() = default;

        ///
        ~UnitState() = default;

        ///
        bool IsInitialized() const { return m_is_initialized; }

        ///
        bool IsDefaultValue() const { return m_is_default_value; }

        ///
        bool IsPublic() const { return m_is_public; }
};

#endif //CONFIGSVC_UNITSTATE_HH

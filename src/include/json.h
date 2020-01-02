#include "types.h"

namespace ice
{
    template<typename T>
    std::pair<bool, T> get_json_primitive(const json11::Json & jsonData,
            const std::string & keyword, const bool throwExceptionWhenMissing)
    {
        json11::Json obj = json_data[keyword];

        std::pair<bool, T> ret(false, T());

        if (!obj.is_null())
        {
            ret = std::make_pair(true, std::move(convert_json_value<T>(obj)));
        }
        else
        {
            if (throwExceptionWhenMissing)
            {
                throw std::runtime_error(
                        "Unexpected parse error.  Missing '" + keyword + "'");
            }
        }
        return std::move(ret);
    }


class JSON
{
public:
    JSON() {}
    virtual ~JSON() {}

    virtual std::string serialize(void ) const
        {
            return ((Json)get_data()).dump();
        }

    virtual Json to_json() const
        {
            const auto & d = get_data();
            Json j = d;
            return j;
        }

    virtual bool empty(void) const = 0;
    virtual void clear(void) = 0;

protected:
    template<typename T>
    T get_value(const std::string & kw) const
        {
            T v;
            std::tie(std::ignore, v) =
                get_json_primitive<T>(get_data(), kw, true);
            return v;
        }

    template <typename T>
    bool has_value(const std::string & kw) const
        {
            bool ret;
            std::tie(ret, std::ignore) =
                get_json_primitive<T>(get_data(), kw, false);
            return ret;
        }

    virtual const Json::object & get_data() const = 0;
    virtual void set_data(const Json & json) = 0;
    virtual void remove_member(const std::string & kw) = 0;

    virtual void parse(const std::string & str) {
        std::string err;
        Json o = Json::parse(str, err);
        Json::object x = o.object_items();
        set_data(x);
    }
};


#define BEGIN_JSON_OBJECT(OBJECT_NAME)                                  \
    class OBJECT_NAME : public JSON {                   \
    private:                                                            \
        Json::object m_data;                                    \
    protected:                                                          \
        virtual const Json::object & get_data() const override {\
            return m_data;                                              \
        }                                                               \
        virtual void set_data(const Json & json) override {     \
            m_data = json.object_items();                               \
        }                                                               \
        virtual void remove_member(const std::string & kw) override {   \
            m_data.erase(kw);                                           \
        }                                                               \
    public:                                                             \
        OBJECT_NAME (const std::string & str) : JSON() { parse(str);}   \
        OBJECT_NAME (const Json & json) : JSON()                \
        {                                                               \
            set_data(json);                                             \
        }                                                               \
        OBJECT_NAME () : JSON() {}                                      \
        bool empty(void) const override                                 \
        {                                                               \
            return m_data.empty();                                      \
        }                                                               \
        void clear(void) override                                       \
        {                                                               \
            m_data.clear();                                             \
        }

#define JSON_PRIMITIVE(NAME, STRING, TYPE)                              \
    private:                                                            \
        static constexpr const char * const NAME##_word = STRING ;      \
        typedef TYPE NAME##_t;                                          \
    public:                                                             \
        bool has_##NAME() const                                         \
        {                                                               \
            return has_value<NAME##_t>(NAME##_word);                     \
        }                                                               \
        NAME##_t get_##NAME() const                                     \
        {                                                               \
            return get_value<NAME##_t>(NAME##_word);                     \
        }                                                               \
        void set_##NAME(const NAME##_t & value)                         \
        {                                                               \
            m_data[NAME##_word] = value;                                \
        };                                                              \
        void set_##NAME(NAME##_t && value)                              \
        {                                                               \
            m_data[NAME##_word] = value;                                \
        };                                                              \
        virtual void remove_##NAME(void)                                \
        {                                                               \
            return remove_member(NAME##_word);                            \
        };

#define JSON_KEY(NAME)                          \
    public:                                     \
        Json key() const                \
        {                                       \
            return get_##NAME();                \
        }                                       \
        static std::string key_name()           \
        {                                       \
            return std::string(NAME##_word);    \
        }

#define JSON_ARRAY(NAME, STRING)                                        \
    private:                                                            \
        static constexpr const char * const NAME##_word = STRING;       \
        typedef Json::array NAME##_t;                           \
    public:                                                             \
        bool has_##NAME() const                                         \
        {                                                               \
            return has_value<NAME##_t>(NAME##_word);                     \
        }                                                               \
        NAME##_t get_##NAME() const                                     \
        {                                                               \
            return get_value<NAME##_t>(NAME##_word);                     \
        }                                                               \
        void set_##NAME(const NAME##_t & value)                         \
        {                                                               \
            m_data[NAME##_word] = value;                                \
        }                                                               \
        void set_##NAME(NAME##_t && value)                              \
        {                                                               \
            m_data[NAME##_word] = value;                                \
        }                                                               \
        void add_to_##NAME(const Json & v)                      \
        {                                                               \
            NAME##_t arr = has_##NAME() ? get_##NAME() : NAME##_t();    \
            NAME##_t::iterator itr = arr.begin();                       \
            while (itr != arr.end()) {                                  \
                if (*itr == v) {                                        \
                    arr.erase(itr);                                     \
                    break;                                              \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            arr.push_back(v);                                           \
            set_##NAME(std::move(arr));                                 \
        }                                                               \
                                                                        \
        void remove_from_##NAME(const Json & v) {               \
            NAME##_t arr = has_##NAME() ? get_##NAME() : NAME##_t();    \
            NAME##_t::iterator itr = arr.begin();                       \
            while (itr != arr.end()) {                                  \
                if (*itr == v) {                                        \
                    arr.erase(itr);                                     \
                    break;                                              \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            set_##NAME(std::move(arr));                                 \
        }                                                               \
        bool contains_##NAME(const Json & v) const {            \
            const NAME##_t arr =                                        \
                has_##NAME() ? get_##NAME() : NAME##_t();               \
            NAME##_t::const_iterator itr = arr.begin();                 \
            while (itr != arr.end()) {                                  \
                if (*itr == v) {                                        \
                    return true;                                        \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            return false;                                               \
        }                                                               \
    private:

#define JSON_ARRAY_ITEMS(NAME, STRING, TYPE)                            \
    private:                                                            \
        static constexpr const char * const NAME##_word = STRING ;      \
        typedef TYPE NAME##_t;                                          \
    public:                                                             \
        bool has_##NAME() const                                         \
        {                                                               \
            return has_value<Json::array>(NAME##_word);          \
        }                                                               \
        Json::array get_##NAME() const                          \
        {                                                               \
            return get_value<Json::array>(NAME##_word);          \
        }                                                               \
        void set_##NAME(const Json::array & value)              \
        {                                                               \
            m_data[NAME##_word] = value;                                \
        }                                                               \
        void set_##NAME(Json::array && value)                   \
        {                                                               \
            m_data[NAME##_word] = value;                                \
        }                                                               \
        void add_to_##NAME(const NAME##_t & v)                          \
        {                                                               \
            Json::array arr =                                   \
                has_##NAME() ? get_##NAME() : Json::array();    \
            Json::array::iterator itr = arr.begin();            \
            const auto & key = v.key();                                 \
            while (itr != arr.end()) {                                  \
                const auto & x = (*itr).object_items();                 \
                const Json & value = x.at(TYPE::key_name());    \
                if (value == key) {                                     \
                    arr.erase(itr);                                     \
                    break;                                              \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            arr.push_back(v.to_json());                                 \
            set_##NAME(std::move(arr));                                 \
        }                                                               \
                                                                        \
        void remove_from_##NAME(const NAME##_t & v) {                   \
            Json::array arr =                                   \
                has_##NAME() ? get_##NAME() : Json::array();    \
            Json::array::iterator itr = arr.begin();            \
            const auto & key = v.key();                                 \
            while (itr != arr.end()) {                                  \
                const auto & x = (*itr).object_items();                 \
                const Json & value = x.at(TYPE::key_name());    \
                if (value == key) {                                     \
                    arr.erase(itr);                                     \
                    break;                                              \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            set_##NAME(std::move(arr));                                 \
        }                                                               \
        bool contains_##NAME(const NAME##_t & v) const {                \
            return contains_##NAME(v.key());                            \
        }                                                               \
        bool contains_##NAME(const Json & key) const {          \
            const Json::array arr =                             \
                has_##NAME() ? get_##NAME() : Json::array();    \
            Json::array::const_iterator itr = arr.begin();      \
            while (itr != arr.end()) {                                  \
                const auto & x = (*itr).object_items();                 \
                const Json & value = x.at(TYPE::key_name());    \
                if (value == key) {                                     \
                    return true;                                        \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            return false;                                               \
        }                                                               \
        NAME##_t get_##NAME(const NAME##_t & v) const {                 \
            return get_##NAME(v.key());                                 \
        }                                                               \
        NAME##_t get_##NAME(const Json & key) const {           \
            const Json::array arr =                             \
                has_##NAME() ? get_##NAME() : Json::array();    \
            Json::array::const_iterator itr = arr.begin();      \
            while (itr != arr.end()) {                                  \
                const auto & x = (*itr).object_items();                 \
                const Json & value = x.at(TYPE::key_name());    \
                if (value == key) {                                     \
                    return NAME##_t(x);                                 \
                }                                                       \
                itr++;                                                  \
            }                                                           \
            return NAME##_t(std::string(""));                           \
        }                                                               \
    private:


#define JSON_NESTED_OBJECT(NAME, STRING, TYPE)                          \
    private:                                                            \
        static constexpr const char * const NAME##_word = STRING;       \
        typedef TYPE NAME##_t;                                          \
    public:                                                             \
        void set_##NAME(const NAME##_t & value)                         \
        {                                                               \
            m_data[NAME##_word] = value.to_json();                      \
        }                                                               \
        void set_##NAME(NAME##_t && value)                              \
        {                                                               \
            m_data[NAME##_word] = value.to_json();                      \
        }                                                               \
        bool has_##NAME() const                                         \
        {                                                               \
            return has_value<Json::object>(NAME##_word);         \
        }                                                               \
        NAME##_t get_##NAME() const                                     \
        {                                                               \
            return NAME##_t(                                            \
                get_value<Json::object>(                         \
                    NAME##_word));                                      \
        }                                                               \
        void remove_##NAME(void)                                        \
        {                                                               \
            return remove_member(NAME##_word);                            \
        }                                                               \
    private:

#define END_JSON_OBJECT };

}
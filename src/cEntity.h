#include <vector>
#include <string>
namespace raven
{
    namespace edb
    {
        /// @brief A pair describing an entity
        /// first is the id
        /// second is a vector of strings containing the attributes
        typedef std::pair<
            int, std::vector<std::string>>
            entityDesc_t;

        class cValue
        {
        public:
            int aid;           /// attribute ID
            int pid;           /// entity ID
            std::string value; /// value of attribute

            void text_from_file();
            std::string text_for_file();
        };
        class cEntity
        {
        public:
            cEntity(
                std::string type,
                const std::vector<int> &vAtts)
                : myType( type ),
                myAtts(vAtts)
            {
            }
            void set(const std::vector<std::string> &vals);
            std::vector<cValue> get() const;

        private:
            std::string myType;
            std::vector<int> myAtts;
            std::vector<std::string> myVals;
        };
        class cEntityDB
        {
        public:
            /// @brief read permanent storage
            /// @param[in] fname file path

            void load(const std::string &fname);

            /// @brief save to permanent storage
            void save();

            /// @brief add a new entity
            /// @param[in] vv attribute values

            void add(const std::vector<std::pair<int, std::string>> &vv);
            void add(const cEntity &e);

            void update(
                const cEntity &e,
                int pid );

            /** @brief update an entity value
             * @param[in] v new value
             *
             * the value that matches input attribute and id will be updated
             * permanent storage is not updated, save() must be called when updates complete
             */

            void update(const cValue &v);

            /// @brief get entity description
            /// @param[in] pid id required
            /// @param[in] vatt attributes of entity type
            /// @return entity description

            entityDesc_t get(
                int pid,
                const std::vector<int> &vatt);

        protected:
            std::vector<cValue> myValue; /// the database
            std::string myfname;
            static int lastPID;
        };
    }
}
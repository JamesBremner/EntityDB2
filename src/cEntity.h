#include <vector>
#include <string>
namespace raven
{
    namespace edb
    {
        /// @brief A pair describing an entity
        /// first is the entity id
        /// second is a vector of strings containing the attributes
        typedef std::pair<
            int, std::vector<std::string>>
            entityDesc_t;

        /// @brief vector of entity descriptions
        typedef std::vector<
            entityDesc_t >
            entityList_t;

        /// @brief An attribute

        class cValue
        {
        public:
            int aid;           /// attribute ID
            int pid;           /// entity ID
            std::string value; /// value of attribute

            // replaces '_' with ' ' in value

            void text_from_file();

            /// @brief prepare text to write to storage file
            /// @return text, with ' ' in value replaced with '_'

            std::string text_for_file();
        };

        /// @brief A base class for all entities

        class cEntity
        {
        public:

            /** CTOR
             * param[in] type name of entity type
             * param[in] vector of attribute types
             */

            cEntity(
                std::string type,
                const std::vector<int> &vAtts)
                : myType( type ),
                myAtts(vAtts)
            {
            }

            /// @brief set attribute values
            /// @param[in] vals values

            void set(const std::vector<std::string> &vals);
            
            /// @brief get attributes
            /// @return vector of attribute values

            std::vector<cValue> get() const;

            const std::string& type() const
            {
                return myType;
            }
            const std::vector<int>& AttibuteIndices() const
            {
                return myAtts;
            }

        private:
            std::string myType;
            std::vector<int> myAtts;
            std::vector<std::string> myVals;
        };
        
        /** @brief A database if entities
         * 
         * Each entity is a set of attributes
         * - attribute ID
         * - entity ID
         * - attribute value
         */

        class cEntityDB
        {
        public:
            /// @brief read permanent storage
            /// @param[in] fname file path

            void load(const std::string &fname);

            /// @brief save to permanent storage using filename from last load()

            void save();

            /// @brief add a new entity
            /// @param[in] vv attribute values

            void add(const std::vector<std::pair<int, std::string>> &vv);

            /// @brief add a new entity
            /// @param[in] e new entity

            void add(const cEntity &e);

            /// @brief update an entity's values
            /// @param[in] e entity with new values
            /// @param pid entity ID to update

            void update(
                const cEntity &e,
                int pid );

            /** @brief update an entity attribute value
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

            /// @brief list of entities of same type
            /// @param[in] e entity of type reuired
            /// @return entity list

            entityList_t entitylist(
                const cEntity& e );
            

        protected:
            std::vector<cValue> myValue; /// the database
            std::string myfname;         /// filename for permanent storage
            static int lastPID;          /// ID of last new entity added
        };
    }
}
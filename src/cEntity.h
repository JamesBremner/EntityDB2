#include <vector>
#include <string>
namespace raven
{
    namespace edb
    {
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
        };
        class cEntityDB
        {
        public:
            void load(const std::string &fname);
            void save();
            void add( const std::vector<std::pair<int,std::string>>& vv );
            void update( const cValue& v );

        protected:
            std::vector<cValue> myValue; /// the database
            std::string myfname;
            static int lastPID;
        };
    }
}
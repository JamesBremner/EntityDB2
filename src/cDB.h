#include "cEntity.h"

    /// @brief The different attributes in database
    enum class eAttribute
    {
        none,
        role,
        name,
        nurselicence,
        service,
        expire,
        certification,
        authorization,
        supplies,
    };

class cDB : public raven::edb::cEntityDB
{
    /// @brief A pair describing a person
    /// first is the person's id
    /// second is a vector of strings containing the person's attributes
    typedef std::pair<
        int, std::vector<std::string>>
        person_t;

    /// @brief A vector describing a subset of persons
    typedef std::vector<person_t> vperson_t;

public:
    cDB();

    /// @brief list of nurses for display, sorted by date
    /// @param[in] att index of attribute to sort by
    /// @return list
    vperson_t nursebyDate(int att);

    /// @brief nurse from display list
    /// @param[in] listIndex index
    /// @return description
    person_t nurselist(int listIndex);

    vperson_t patientbyDate(int att);

    person_t patientlist(int listIndex);

private:

    vperson_t myNurseList;       /// the displayed nurses
    vperson_t myPatientList;     /// the displayed patients

    /// @brief sort person list into attribute date order
    /// @param[in/out] list
    /// @param att attribute index in person_t
    void sort(
        vperson_t &list,
        int att);
    void load();
    long long  secs(
        const person_t& p,
        int index);
};

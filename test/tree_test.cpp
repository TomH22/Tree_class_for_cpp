#include <gtest/gtest.h>
#include "Staff.h"
#include <string>

using namespace std;
using namespace henn;

class TreeTestSuite : public ::testing::Test {
 protected:
};

/**
Add one element to the tree and test its value.
**/
TEST_F(TreeTestSuite, AddOnePersonData) {
	struct Person {
	private:
		std::wstring name;
	public:
		Person() = default;
		Person(std::wstring _name) : name(_name) {};
		std::wstring GetName() const { return name; };
	};

	Person marco(L"Marco");

	TreeModel<Person> treeModel(marco);
	EXPECT_EQ(L"Marco", treeModel.GetTreeItem().m_Item.GetName());
}

/**
Add 18 elements to the tree and test the size.
**/
TEST_F(TreeTestSuite, AddStaffData) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	Staff quaas(L"Quaas", Staff::RANK::SERGANT);
	Staff cox(L"Cox", Staff::RANK::SERGANT);
	Staff muller(L"Muller", Staff::RANK::CORPORAL);
	Staff lugk(L"Lugk", Staff::RANK::CORPORAL);

	TreeModel<Staff> treeModel(loos);
	treeModel.Append(quaas, loos);
	treeModel.Append(cox, loos);
	treeModel.Append(Staff(L"Brown", Staff::RANK::SERGANT), loos);

	treeModel.Append(Staff(L"Epheser", Staff::RANK::CORPORAL), cox);
	treeModel.Append(Staff(L"Haupt", Staff::RANK::CORPORAL), cox);

	treeModel.Append(lugk, quaas);
	treeModel.Append(muller, quaas);

	treeModel.Append(Staff(L"Schmidt", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Titze", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Uhlig", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Ebert", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Hartmann", Staff::RANK::PRIVATE), muller);

	treeModel.Append(Staff(L"Pretsch", Staff::RANK::PRIVATE), lugk);
	treeModel.Append(Staff(L"Dressler", Staff::RANK::PRIVATE), lugk);
	treeModel.Append(Staff(L"Pietsch", Staff::RANK::PRIVATE), lugk);
	treeModel.Append(Staff(L"Lippa", Staff::RANK::PRIVATE), lugk);
	treeModel.Append(Staff(L"Neumann", Staff::RANK::PRIVATE), lugk);

	EXPECT_EQ(18, treeModel.Size());
}

#include "Staff.h"

#include <gtest/gtest.h>

#include <string>
#include <memory>

using namespace std;
using namespace henn;

class TreeTestSuite : public ::testing::Test {
 protected:
};

/**
Add one element to the tree and test its value.
*/
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
	EXPECT_EQ(L"Marco", treeModel.GetTreeItem()->m_Item->GetName());
}

/**
Add 18 elements to the tree and test the size.
*/
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

/**
Add an element to return the first element.
*/
TEST_F(TreeTestSuite, TestGet1) {
	struct Person {
	private:
		std::wstring name;
	public:
		Person() = default;
		Person(std::wstring _name) : name(_name) {};
		std::wstring GetName() const { return name; };
	};

	Person marco(L"Thomas");
	TreeModel<Person> treeModel(marco);
	EXPECT_EQ(L"Thomas", treeModel.Get(0).GetName());
}

TEST_F(TreeTestSuite, TestSortSecondColumn) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	Staff quaas(L"Quaas", Staff::RANK::SERGANT);
	Staff cox(L"Cox", Staff::RANK::SERGANT);

	TreeModel<Staff> treeModel(loos);
	treeModel.Append(quaas, loos);
	treeModel.Append(cox, loos);
	treeModel.Append(Staff(L"Brown", Staff::RANK::SERGANT), loos);

	treeModel.SortSecondColumn();
	EXPECT_EQ(L"Brown", treeModel.GetTreeItem()->m_Children[0]->m_Item->GetName());
}

TEST_F(TreeTestSuite, TestSortAllColumns) {
	Staff a(L"A", Staff::RANK::STAFF_SERGANT);
	Staff b(L"B", Staff::RANK::SERGANT);
	Staff c(L"C", Staff::RANK::SERGANT);
	Staff d(L"D", Staff::RANK::CORPORAL);
	Staff e(L"E", Staff::RANK::CORPORAL);


	TreeModel<Staff> treeModel(a);
	treeModel.Append(c, a);
	treeModel.Append(b, a);

	treeModel.Append(e, c);
	treeModel.Append(d, c);

	treeModel.SortAllColumns();
	EXPECT_EQ(L"B", treeModel.GetTreeItem()->m_Children[0]->m_Item->GetName());
}

TEST_F(TreeTestSuite, NegativeTestSortAllColumns) {
	Staff a(L"A", Staff::RANK::STAFF_SERGANT);
	Staff b(L"B", Staff::RANK::SERGANT);
	Staff c(L"C", Staff::RANK::SERGANT);
	Staff d(L"D", Staff::RANK::CORPORAL);
	Staff e(L"E", Staff::RANK::CORPORAL);

	TreeModel<Staff> treeModel(a);
	treeModel.Append(c, a);
	treeModel.Append(b, a);

	treeModel.Append(e, c);
	treeModel.Append(d, c);

	//treeModel.SortAllColumns();
	EXPECT_NE(L"B", treeModel.GetTreeItem()->m_Children[0]->m_Item->GetName());
}

TEST_F(TreeTestSuite, TestTopDownIterator) {
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

	auto i = TopDownIterator(treeModel);

	int counter = 0;
	for (i.First(); !i.IsDone(); i.Next())
	{
		counter++;
	}
	EXPECT_EQ(17, counter);
}

TEST_F(TreeTestSuite, TestStaffIteratorTopItem) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	Staff quaas(L"Quaas", Staff::RANK::SERGANT);
	Staff cox(L"Cox", Staff::RANK::SERGANT);
	Staff muller(L"Muller", Staff::RANK::CORPORAL);
	Staff lugk(L"Lugk", Staff::RANK::CORPORAL);

	TreeModel<Staff> treeModel(loos);
	treeModel.Append(quaas, loos);
	treeModel.Append(cox, loos);
	treeModel.Append(Staff(L"Brown", Staff::RANK::SERGANT), loos);

	auto i = StaffIteratorTopItem(treeModel);

	int counter = 0;
	for (i.First(); !i.IsDone(); i.Next())
	{
		counter++;
	}
	EXPECT_EQ(1, counter);
}

TEST_F(TreeTestSuite, TestStaffTopDownIterator2) {
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

	auto i = StaffTopDownIterator2(treeModel, true, true, true);

	int counter = 0;
	for (i.First(); !i.IsDone(); i.Next())
	{
		counter++;
		if (counter == 4)
		{
			EXPECT_EQ(i.CurrentItem().GetName(), L"Brown (second output)");
			break;
		}
	}
}

TEST_F(TreeTestSuite, TestIsNull) {
	TreeModel<Staff> treeModel{};
	EXPECT_EQ(true, treeModel.IsNull());
	EXPECT_EQ(0, treeModel.Size());
}

TEST_F(TreeTestSuite, NegativeTestIsNull) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	TreeModel<Staff> treeModel(loos);
	EXPECT_NE(true, treeModel.IsNull());
	EXPECT_NE(0, treeModel.Size());
}

TEST_F(TreeTestSuite, TestJsonSerializationGerman) {
	Staff muller(L"Müller", Staff::RANK::CORPORAL);
	json j = static_cast<json>(muller);
	Staff alsoMuller = j.get<Staff>();
	EXPECT_TRUE(muller == alsoMuller);
}

TEST_F(TreeTestSuite, TestJsonSerialization1) {
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

	json j = treeModel.GetJson();

	TreeModel<Staff> alsoTreeModel(j);
	EXPECT_TRUE(treeModel == alsoTreeModel);
	EXPECT_FALSE(treeModel != alsoTreeModel);
}

TEST_F(TreeTestSuite, NegativeTestJsonSerialization1) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	Staff quaas(L"Quaas", Staff::RANK::SERGANT);
	Staff cox(L"Cox", Staff::RANK::SERGANT);
	Staff muller(L"Muller", Staff::RANK::CORPORAL);
	Staff lugk(L"Lugk", Staff::RANK::CORPORAL);

	TreeModel<Staff> treeModel(loos);
	{
		treeModel.Append(quaas, loos);
		treeModel.Append(cox, loos);
		treeModel.Append(Staff(L"Brown", Staff::RANK::SERGANT), loos);

		treeModel.Append(Staff(L"Epheser", Staff::RANK::CORPORAL), cox);
		treeModel.Append(Staff(L"Haupt", Staff::RANK::CORPORAL), cox);

		treeModel.Append(lugk, quaas);
		treeModel.Append(muller, quaas);
	}

	TreeModel<Staff> notAlsotreeModel(loos);
	{
		notAlsotreeModel.Append(quaas, loos);
		notAlsotreeModel.Append(cox, loos);
		notAlsotreeModel.Append(Staff(L"Lange", Staff::RANK::SERGANT), loos);

		notAlsotreeModel.Append(Staff(L"Epheser", Staff::RANK::CORPORAL), cox);
		notAlsotreeModel.Append(Staff(L"Haupt", Staff::RANK::CORPORAL), cox);

		notAlsotreeModel.Append(lugk, quaas);
		notAlsotreeModel.Append(muller, quaas);
	}

	json j = treeModel.GetJson();
	TreeModel<Staff> alsoTreeModel(j);
	EXPECT_FALSE(notAlsotreeModel == alsoTreeModel);
	EXPECT_TRUE(notAlsotreeModel != alsoTreeModel);
}

TEST_F(TreeTestSuite, TestGetFromStructure) {
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

	Staff schmidt(L"Schmidt", Staff::RANK::PRIVATE);
	treeModel.Append(Staff(L"Titze", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Uhlig", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Ebert", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Hartmann", Staff::RANK::PRIVATE), muller);
	treeModel.Append(schmidt, muller);

	std::vector structure{ loos, quaas};
	auto& alsoQuaas = treeModel.Get(structure);
	EXPECT_EQ(*alsoQuaas, quaas);

	std::vector structure2{ loos, quaas, muller, schmidt };
	auto& alsoSchmidt = treeModel.Get(structure2);
	EXPECT_EQ(*alsoSchmidt, schmidt);
}


TEST_F(TreeTestSuite, TestGetFromStructureNotFound) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	Staff muller(L"Muller", Staff::RANK::CORPORAL);
	Staff lugk(L"Lugk", Staff::RANK::CORPORAL);

	TreeModel<Staff> treeModel(loos);
	treeModel.Append(Staff(L"Brown", Staff::RANK::SERGANT), loos);

	treeModel.Append(muller, loos);

	Staff titze(L"Titze", Staff::RANK::PRIVATE);
	treeModel.Append(titze, muller);
	treeModel.Append(Staff(L"Uhlig", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Ebert", Staff::RANK::PRIVATE), muller);
	treeModel.Append(Staff(L"Hartmann", Staff::RANK::PRIVATE), muller);

	Staff empty{};
	Staff quaas(L"Quaas", Staff::RANK::SERGANT);
	std::vector structure{ loos, quaas };
	auto& alsoEmpty = treeModel.Get(structure);
	EXPECT_EQ(*alsoEmpty, empty);
	EXPECT_NE(*alsoEmpty, quaas);

	Staff schmidt(L"Schmidt", Staff::RANK::PRIVATE);
	std::vector structure2{ loos, muller, schmidt };
	auto& alsoEmpty2 = treeModel.Get(structure2);
	EXPECT_EQ(empty, *alsoEmpty2);
	EXPECT_NE(schmidt, *alsoEmpty2);

	std::vector structure3{ loos, loos };
	auto& alsoEmpty3 = treeModel.Get(structure3);
	EXPECT_EQ(empty, *alsoEmpty3);

	std::vector structure4{ loos, titze };
	auto& alsoEmpty4 = treeModel.Get(structure4);
	EXPECT_EQ(empty, *alsoEmpty4);
}

TEST_F(TreeTestSuite, TestGetFromStructureAndChange) {
	Staff loos(L"Loos", Staff::RANK::STAFF_SERGANT);
	Staff quaas(L"Quaas", Staff::RANK::SERGANT);

	TreeModel<Staff> treeModel(loos);
	treeModel.Append(quaas, loos);

	Staff quasBase(L"Quaas", Staff::RANK::PRIVATE);
	std::vector structure{ loos, quasBase };
	auto& alsoQuaas = treeModel.Get(structure);
	EXPECT_EQ(*alsoQuaas, quaas);
	EXPECT_EQ(alsoQuaas->GetRank(), Staff::RANK::SERGANT);
	alsoQuaas->SetRank(Staff::RANK::CORPORAL);

	
	std::vector structure2{ loos, quasBase };
	auto& alsoQuaasSecondGet = treeModel.Get(structure2);
	EXPECT_EQ(*alsoQuaasSecondGet, quaas);
	EXPECT_EQ(alsoQuaasSecondGet->GetRank(), Staff::RANK::CORPORAL);
}
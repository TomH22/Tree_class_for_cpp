#include "Layer.h"

#include <gtest/gtest.h>

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>

using namespace std;
using namespace henn;

class TreeTestSuite2 : public ::testing::Test {
 protected:
	 void SetUp() override
	 {
		 std::string test_data_path = CMAKE_BINARY_DIR "/test_data/";
		 std::ifstream file(test_data_path + "sw.json");
		 //file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<char>));

		 json j;
		 file >> j;

		 henn::TreeModel<Layer> treeModel(j);
		 m_TreeModel = treeModel;
	 }
protected:
	henn::TreeModel<Layer> m_TreeModel;
};

/**
Add one element to the tree and test its value.
*/
TEST_F(TreeTestSuite2, ReadTestData) {
	EXPECT_EQ(280, m_TreeModel.Size());
}

TEST_F(TreeTestSuite2, AddTestSizeLeaveNodes)
{
	auto ln = m_TreeModel.SizeLeaveNodes();
	EXPECT_EQ(249, m_TreeModel.SizeLeaveNodes());
}

TEST_F(TreeTestSuite2, TestGet1)
{
	EXPECT_EQ("Siedlungsweg", m_TreeModel.Get(0).name);
}

TEST_F(TreeTestSuite2, TestSortSecondColumn)
{
	m_TreeModel.SortSecondColumn();
	auto name = m_TreeModel.GetTreeItem()->m_Children[0]->m_Item->name;
	EXPECT_EQ("DG", name);
}

TEST_F(TreeTestSuite2, TestSortAllColumns)
{
	m_TreeModel.SortAllColumns();
	auto& item = m_TreeModel.GetTreeItem()->m_Children[0]->m_Children[0]->m_Children[0]->m_Item;
	auto name = item->name;
	EXPECT_EQ("Boden", name);
}

TEST_F(TreeTestSuite2, NegativeTestSortAllColumns)
{
	//m_TreeModel.SortAllColumns();
	auto name = m_TreeModel.GetTreeItem()->m_Children[0]->m_Children[0]->m_Children[0]->m_Item->name;
	EXPECT_NE("Boden", name);
}

TEST_F(TreeTestSuite2, TestTopDownIterator)
{
	auto i = TopDownIterator(m_TreeModel);
	int counter = 0;
	for (i.First(); !i.IsDone(); i.Next())
	{
		counter++;
	}
	EXPECT_EQ(280, counter);
}

TEST_F(TreeTestSuite2, TestStaffIteratorTopItem)
{
	auto i = LayerListIterator(m_TreeModel, true);

	int counter = 0;
	for (i.First(); !i.IsDone(); i.Next())
	{
		counter++;
	}
	EXPECT_EQ(1, counter);
}


TEST_F(TreeTestSuite2, TestIsNull)
{
	TreeModel<Layer> treeModel{};
	EXPECT_EQ(true, treeModel.IsNull());
	EXPECT_EQ(0, treeModel.Size());
}

TEST_F(TreeTestSuite2, NegativeTestIsNull)
{
	EXPECT_NE(true, m_TreeModel.IsNull());
	EXPECT_NE(0, m_TreeModel.Size());
}

TEST_F(TreeTestSuite2, TestJsonSerialization1)
{
	json j = m_TreeModel.GetJson();

	TreeModel<Layer> alsoTreeModel(j);
	EXPECT_TRUE(m_TreeModel == alsoTreeModel);
	EXPECT_FALSE(m_TreeModel != alsoTreeModel);
}

TEST_F(TreeTestSuite2, NegativeTestJsonSerialization1)
{
	json j = m_TreeModel.GetJson();

	TreeModel<Layer> notAlsotreeModel(j);
	m_TreeModel.GetTreeItem()->m_Children[0]->m_Children[0]->m_Children[0]->m_Item->name = "Boden neu";

	EXPECT_FALSE(notAlsotreeModel == m_TreeModel);
	EXPECT_TRUE(notAlsotreeModel != m_TreeModel);
}

TEST_F(TreeTestSuite2, TestGetFromStructure)
{
	Layer l1;
	l1.name = "Siedlungsweg";
	Layer l2;
	l2.name = "Treppenanlage";
	Layer l3;
	l3.name = "EG-OG";
	Layer l4;
	l4.name = "Ortbetontkern";
	Layer l5;
	l5.name = "KG";
	Layer l6;
	l6.name = "Flur";
	Layer l7;
	l7.name = "EG";
	Layer l8;
	l8.name = "Grundriss";

	std::vector structure1{ l1, l2, l3, l4 };
	auto& ortbetontkern = m_TreeModel.Get(structure1);
	EXPECT_EQ(*ortbetontkern, l4);

	std::vector structure2{ l1, l5, l6, l8 };
	auto& grundriss1 = m_TreeModel.Get(structure2);
	EXPECT_EQ(*grundriss1, l8);
	EXPECT_EQ(grundriss1->note, "Dieser Grundriss ist speziell!");

	std::vector structure3{ l1, l7, l6, l8 };
	auto& grundriss2 = m_TreeModel.Get(structure3);
	EXPECT_EQ(*grundriss2, l8);
	EXPECT_EQ(grundriss2->note, "");
}


TEST_F(TreeTestSuite2, TestGetFromStructureNotFound)
{
	Layer l1;
	l1.name = "Siedlungsweg";
	Layer l6;
	l6.name = "Toilette";
	Layer l7;
	l7.name = "EG";
	Layer l8;
	l8.name = "Grundriss";
	Layer emtpy;

	std::vector structure3{ l1, l7, l6, l8 };
	auto& grundriss2 = m_TreeModel.Get(structure3);
	EXPECT_EQ(*grundriss2, emtpy);
	EXPECT_NE(*grundriss2, l8);
}

TEST_F(TreeTestSuite2, TestGetFromStructureAndChange)
{
	Layer l1;
	l1.name = "Siedlungsweg";
	Layer l2;
	l2.name = "Treppenanlage";
	Layer l3;
	l3.name = "EG-OG";
	Layer l4;
	l4.name = "Ortbetontkern";
	Layer l5;
	l5.name = "KG";
	Layer l6;
	l6.name = "Flur";
	Layer l7;
	l7.name = "EG";
	Layer l8;
	l8.name = "Grundriss";

	std::vector structure2{ l1, l5, l6, l8 };
	auto& grundriss1 = m_TreeModel.Get(structure2);
	EXPECT_EQ(*grundriss1, l8);
	EXPECT_EQ(grundriss1->note, "Dieser Grundriss ist speziell!");

	grundriss1->note = "Der Grundriss ist jetzt nicht OK.";

	std::vector structure3{ l1, l5, l6, l8 };
	auto& grundriss1Too = m_TreeModel.Get(structure3);
	EXPECT_EQ(grundriss1Too->note, "Der Grundriss ist jetzt nicht OK.");
}

TEST_F(TreeTestSuite2, TestUpdatePropertyForAllItems)
{
	auto treeModelClone = m_TreeModel;

	auto updateFunction = [](std::unique_ptr<Layer>& layer)
		{
			layer->markForExport = true;
		};

	treeModelClone.UpdatePropertyForAllItems(updateFunction);

	auto i = LayerListIterator(treeModelClone);
	int counter = 0;
	for (i.First(); !i.IsDone(); i.Next())
	{
		EXPECT_TRUE(i.CurrentItem().markForExport);
		counter++;
	}
	EXPECT_EQ(counter, 280);
}

TEST_F(TreeTestSuite2, TestGetPath2)
{
	auto path = m_TreeModel.GetPath(5);

	EXPECT_EQ(path[1].name, "KG");
	EXPECT_EQ(path[3].name, "Decke");


	auto path2 = m_TreeModel.GetPath(48);

	EXPECT_EQ(path2[3].name, "Wand 4");

	auto path3 = m_TreeModel.GetPath(50);

	EXPECT_EQ(path3[1].name, "KG");
	EXPECT_EQ(path3[2].name, "Toilette");
	EXPECT_EQ(path3[3].name, "Grundriss");
}

TEST_F(TreeTestSuite2, TestFindItems)
{
	auto findFunction = [](std::unique_ptr<Layer>& layer) -> bool
		{
			return (layer->name == "Schuhraum");
		};

	auto result = m_TreeModel.FindItems(findFunction);
	EXPECT_EQ(result[0]->name, "Schuhraum");
}
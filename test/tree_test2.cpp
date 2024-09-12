#include "Layer.h"

#include <gtest/gtest.h>

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>

class TreeTestSuite2 : public ::testing::Test {
 protected:
	 void SetUp() override
	 {
		 std::string test_data_path = CMAKE_BINARY_DIR "/test_data/";

		 {
			 std::ifstream file(test_data_path + "sw.unique.json");

			 json j;
			 file >> j;

			 henn::TreeModel<Layer> treeModel(j);
			 m_TreeModel = treeModel;
		 }

		 //{
			// std::ifstream file(test_data_path + "sw.unique.json");

			// json j;
			// file >> j;

			// henn::TreeModel<Layer> treeModel(j);
			// m_TreeModelUnique = treeModel;
		 //}
	 }
protected:
	henn::TreeModel<Layer> m_TreeModel;
	//henn::TreeModel<Layer> m_TreeModelUnique;
};



/**
Add one element to the tree and test its value.
*/
TEST_F(TreeTestSuite2, ReadTestData) {
	EXPECT_EQ(266, m_TreeModel.Size());
}

TEST_F(TreeTestSuite2, AddTestSizeLeaveNodes)
{
	auto ln = m_TreeModel.SizeLeaveNodes();
	EXPECT_EQ(230, m_TreeModel.SizeLeaveNodes());
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
	EXPECT_EQ(266, counter);
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
	l1.nameUnique = "1";
	Layer l2;
	l2.name = "Treppenanlage";
	l2.nameUnique = "18HDE_lsjDnve5mxlE1HrY";
	Layer l3;
	l3.name = "EG-OG";
	l3.nameUnique = "1rp3oqi0vB9AVVN7qgV7ks";
	Layer l4;
	l4.name = "Ortbetontkern";
	l4.nameUnique = "1cCdXTG5j7e9vxRDmBhN3a";
	Layer l5;
	l5.name = "KG";
	l5.nameUnique = "1Mdo70w4f5nuiyKunWpKwc";
	Layer l6;
	l6.name = "Flur";
	l6.nameUnique = "2j9rw6QEz4qwOFA71fsPP4";
	Layer l7;
	l7.name = "EG";
	l7.nameUnique = "3Zbji619z74PSwzRSx1AOB";
	Layer l8;
	l8.name = "Grundriss";
	l8.nameUnique = "1lo5Iyn8r1OwY0G4Y6WSPu";

	std::vector structure1{ l1, l2, l3, l4 };
	auto& ortbetontkern = m_TreeModel.Get(structure1);
	EXPECT_EQ(*ortbetontkern, l4);

	std::vector structure2{ l1, l5, l6, l8 };
	auto& grundriss1 = m_TreeModel.Get(structure2);
	EXPECT_EQ(*grundriss1, l8);
	EXPECT_EQ(grundriss1->note, "Dieser Grundriss ist speziell!");

	l6.nameUnique = "1dVWiUg1n5Nv4GHzMi$1SX";
	l8.nameUnique = "2LDm5vxg5EBgEqWY0BRYhe";
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
	l1.nameUnique = "1";
	Layer l2;
	l2.name = "Treppenanlage";
	l2.nameUnique = "18HDE_lsjDnve5mxlE1HrY";
	Layer l3;
	l3.name = "EG-OG";
	l3.nameUnique = "1rp3oqi0vB9AVVN7qgV7ks";
	Layer l4;
	l4.name = "Ortbetontkern";
	l4.nameUnique = "1cCdXTG5j7e9vxRDmBhN3a";
	Layer l5;
	l5.name = "KG";
	l5.nameUnique = "1Mdo70w4f5nuiyKunWpKwc";
	Layer l6;
	l6.name = "Flur";
	l6.nameUnique = "2j9rw6QEz4qwOFA71fsPP4";
	Layer l8;
	l8.name = "Grundriss";
	l8.nameUnique = "1lo5Iyn8r1OwY0G4Y6WSPu";

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
	EXPECT_EQ(counter, 266);
}

TEST_F(TreeTestSuite2, TestGetPath2)
{
	auto path = m_TreeModel.GetPath(5);

	EXPECT_EQ(path[1].name, "KG");
	EXPECT_EQ(path[3].name, "Decke");


	auto path2 = m_TreeModel.GetPath(30);

	EXPECT_EQ(path2[3].name, "Wand 3");
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

TEST_F(TreeTestSuite2, TestAddSameTreesInTrees)
{
	Layer fEG;
	fEG.name = "Flur";
	fEG.nameUnique = "1dVWiUg1n5Nv4GHzMi$1SX";
	Layer fKG;
	fKG.name = "Flur";
	fKG.nameUnique = "2j9rw6QEz4qwOFA71fsPP4";
	Layer l1;
	l1.name = "Siedlungsweg";
	l1.nameUnique = "1";
	Layer l2;
	l2.name = "KG";
	l2.nameUnique = "1Mdo70w4f5nuiyKunWpKwc";
	Layer l3;
	l3.name = "EG";
	l3.nameUnique = "3Zbji619z74PSwzRSx1AOB";

	std::vector structure1{ l1, l2, fKG };
	auto& flur1 = m_TreeModel.Get(structure1);

	std::vector structure2{ l1, l3, fEG };
	auto& flur2 = m_TreeModel.Get(structure2);

	Layer l6;
	l6.name = "Riss_1";
	m_TreeModel.Append(l6, *flur2, true);
	flur2->markForExport = true;

	if(false)
	{
		std::string test_data_path = CMAKE_BINARY_DIR "/test_data/";
		auto j = m_TreeModel.GetJson();
		std::ofstream file(test_data_path + "out.json");
		file << j;
	}

	// "Flur" in KG should not be changed.
	{
		auto upTreeItem = std::make_unique<TreeModel<Layer>::TreeItem>();
		auto ret = m_TreeModel.GetCopy(fKG, upTreeItem);

		auto m = std::make_unique<TreeModel<Layer>>(move(upTreeItem));
		auto i = LayerListIterator(*m);
		int counter = 0;
		for (i.First(); !i.IsDone(); i.Next())
		{
			auto iLayer = i.CurrentItem();

			EXPECT_NE(iLayer.name, "Riss_1");
			EXPECT_FALSE(iLayer.markForExport);
			counter++;
		}
		EXPECT_EQ(counter, 12);
	}

	// "Flur" in EG should be changed.
	{
		auto upTreeItem = std::make_unique<TreeModel<Layer>::TreeItem>();
		auto ret = m_TreeModel.GetCopy(fEG, upTreeItem);

		auto m = std::make_unique<TreeModel<Layer>>(move(upTreeItem));
		auto i = LayerListIterator(*m);
		int counter = 0;
		bool rissFound = false;
		bool somethingMarkForExport = false;
		for (i.First(); !i.IsDone(); i.Next())
		{
			auto iLayer = i.CurrentItem();

			if (iLayer.name == "Riss_1")
				rissFound = true;

			if (iLayer.markForExport)
				somethingMarkForExport = true;
			counter++;
		}
		EXPECT_EQ(counter, 10);
		EXPECT_TRUE(rissFound);
		EXPECT_TRUE(somethingMarkForExport);
	}
}

TEST_F(TreeTestSuite2, TestAddSameTreesInTreesWithoutUniqueName)
{
	Layer fEG;
	fEG.name = "Flur";
	Layer fKG;
	fKG.name = "Flur";
	Layer l1;
	l1.name = "Siedlungsweg";
	Layer l2;
	l2.name = "KG";
	Layer l3;
	l3.name = "EG";

	std::vector structure1{ l1, l2, fKG };
	auto& flur1 = m_TreeModel.Get(structure1);

	std::vector structure2{ l1, l3, fEG };
	auto& flur2 = m_TreeModel.Get(structure2);

	Layer l6;
	l6.name = "Riss_1";
	m_TreeModel.Append(l6, *flur2, true);
	flur2->markForExport = true;

	if (false)
	{
		std::string test_data_path = CMAKE_BINARY_DIR "/test_data/";
		auto j = m_TreeModel.GetJson();
		std::ofstream file(test_data_path + "out.json");
		file << j;
	}

	// "Flur" in KG should not be changed.
	{
		auto upTreeItem = std::make_unique<TreeModel<Layer>::TreeItem>();
		fKG.nameUnique = "2j9rw6QEz4qwOFA71fsPP4";// here, I need the concrete "KG/Flur"
		auto ret = m_TreeModel.GetCopy(fKG, upTreeItem);

		auto m = std::make_unique<TreeModel<Layer>>(move(upTreeItem));
		auto i = LayerListIterator(*m);
		int counter = 0;
		for (i.First(); !i.IsDone(); i.Next())
		{
			auto iLayer = i.CurrentItem();

			EXPECT_NE(iLayer.name, "Riss_1");
			EXPECT_FALSE(iLayer.markForExport);
			counter++;
		}
		EXPECT_EQ(counter, 12);
	}

	// "Flur" in EG should be changed.
	{
		auto upTreeItem = std::make_unique<TreeModel<Layer>::TreeItem>();
		fEG.nameUnique = "1dVWiUg1n5Nv4GHzMi$1SX";
		auto ret = m_TreeModel.GetCopy(fEG, upTreeItem);

		auto m = std::make_unique<TreeModel<Layer>>(move(upTreeItem));
		auto i = LayerListIterator(*m);
		int counter = 0;
		bool rissFound = false;
		bool somethingMarkForExport = false;
		for (i.First(); !i.IsDone(); i.Next())
		{
			auto iLayer = i.CurrentItem();

			if (iLayer.name == "Riss_1")
				rissFound = true;

			if (iLayer.markForExport)
				somethingMarkForExport = true;
			counter++;
		}
		EXPECT_EQ(counter, 10);
		EXPECT_TRUE(rissFound);
		EXPECT_TRUE(somethingMarkForExport);
	}
}

TEST_F(TreeTestSuite2, TestNewModelFromTreeItem)
{
	Layer l;
	l.name = "Treppenanlage";
	l.nameUnique = "18HDE_lsjDnve5mxlE1HrY";

	auto treppenanlageTreeItem = std::make_unique<TreeModel<Layer>::TreeItem>();
	auto ret = m_TreeModel.GetCopy(l, treppenanlageTreeItem);

	auto treppenanlageTreeModel = std::make_unique<TreeModel<Layer>>(move(treppenanlageTreeItem));
	auto size = treppenanlageTreeModel->Size();
	EXPECT_EQ(size, 82);
}
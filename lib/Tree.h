#pragma once
#include "nlohmann/json.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <exception>
#include <memory>

using json = nlohmann::json;

/**
@file Tree.h
@brief Tree structure
@note
Root item must be set during initalisation.
Item should define the "<", ">=", "==" and "!=" operators.
*/

namespace henn
{
#pragma region TreeModel
template <class Item>
struct TreeModel
{
public:
	struct TreeItem
	{
		std::unique_ptr<Item> m_Item;
		std::vector<std::unique_ptr<TreeItem>> m_Children;
		bool IsLeaveNode()
		{
			return m_Children.size() == 0;
		};
		bool operator<(const TreeItem& other) const
		{
			return (*this->m_Item < *other.m_Item);
		};
		bool operator==(const TreeItem& other) const
		{
			// First, compare the items themselves
			if (!(*m_Item == *other.m_Item))
				return false;

			// Then compare the number of children
			if (m_Children.size() != other.m_Children.size())
				return false;

			// Finally, compare each child
			for (size_t i = 0; i < m_Children.size(); ++i)
			{
				if (!(*m_Children[i] == *other.m_Children[i]))
					return false;
			}

			// Items and all children are equal
			return true;
		};
		bool operator!=(const TreeItem& other) const
		{
			return !(*this == other);
		}
	};
public:
	TreeModel()
	{
		m_nullItemPtr = std::make_unique<Item>();
	};
	~TreeModel() = default;
	TreeModel(const TreeModel& other);
	TreeModel(const json& j);
	TreeModel(Item _item);
	TreeModel& operator=(const TreeModel& other);
	bool operator==(const TreeModel& other)
	{
		return *m_treeItem == *other.m_treeItem;
	};
	bool operator!=(const TreeModel& other)
	{
		return !(*this == other);
	};
	bool Append(Item _item, Item _parentItem, const bool after = true);
	Item Get(const size_t);
	std::vector<Item> GetPath(const size_t index);
	std::unique_ptr<Item>& Get(std::vector<Item> structure);
	std::unique_ptr<TreeItem>& GetTreeItem()
	{
		return m_treeItem;
	};
	size_t Size() const;
	size_t SizeLeaveNodes() const;
	void SortSecondColumn();
	void SortAllColumns();
	void Clear()
	{
		m_treeItem.reset();
	};
	bool IsNull() const;
	json GetJson() const;
	template <typename UpdateFunction>
	void UpdatePropertyForAllItems(UpdateFunction updateFunction)
	{
		if (!m_treeItem)
			return;

		updatePropertyRecursive(m_treeItem, updateFunction);
	}
	template <typename FindFunction>
	std::vector<std::unique_ptr<Item>> FindItems(FindFunction findFunction)
	{
		if (!m_treeItem)
			return std::vector<std::unique_ptr<Item>>{};

		return findItemsRecursive(m_treeItem, findFunction);
	}
private:
	bool addRecursive(std::unique_ptr<TreeItem>&, Item, Item, const bool after);
	bool getRecursive(std::unique_ptr<TreeItem>&, Item&, const size_t, size_t&);
	bool getRecursivePath(std::unique_ptr<TreeItem>& currentItem, const size_t targetIndex, size_t& currentIndex, std::vector<Item>& path);
	size_t sizeRecursive(const std::unique_ptr<TreeItem>&, bool) const;
	void sortColumnsRecursive(std::unique_ptr<TreeItem>&);
	void copyRecursive(std::unique_ptr<TreeItem>& dest, const std::unique_ptr<TreeItem>& src);
	void from_json_recursive(std::unique_ptr<TreeItem>& treeItem, const json& j);
	template <typename UpdateFunction>
	void updatePropertyRecursive(std::unique_ptr<TreeItem>& currentItem, UpdateFunction updateFunction)
	{
		updateFunction(currentItem->m_Item);
		for (auto& child : currentItem->m_Children)
		{
			updatePropertyRecursive(child, updateFunction);
		}
	}
	template <typename FindFunction>
	std::vector<std::unique_ptr<Item>> findItemsRecursive(std::unique_ptr<TreeItem>& currentItem, FindFunction findFunction)
	{
		std::vector<std::unique_ptr<Item>> result{};

		if (findFunction(currentItem->m_Item))
			result.emplace_back(std::move(std::make_unique<Item>(*currentItem->m_Item)));

		for (auto& child : currentItem->m_Children)
		{
			auto ret = findItemsRecursive(child, findFunction);
			for (auto& e : ret)
			{
				result.emplace_back(std::move(std::make_unique<Item>(*e)));
			}
		}

		return result;
	}
private:
	std::unique_ptr<TreeItem> m_treeItem;
	std::unique_ptr<Item> m_nullItemPtr;
};

template<typename Item>
TreeModel<Item>::TreeModel(const TreeModel& other)
{
	m_nullItemPtr = std::make_unique<Item>();

	// Perform a deep copy of the tree
	if (other.m_treeItem)
	{
		m_treeItem = std::make_unique<TreeItem>();
		copyRecursive(m_treeItem, other.m_treeItem);
	}
}

template<typename Item>
TreeModel<Item>::TreeModel(const json& j)
{
	m_nullItemPtr = std::make_unique<Item>();


	if (!j.is_null() && j.find("item") != j.end())
	{
		m_treeItem = std::make_unique<TreeItem>();
		from_json_recursive(m_treeItem, j);
	}
};

template<typename Item>
TreeModel<Item>::TreeModel(Item _item)
{
	m_nullItemPtr = std::make_unique<Item>();

	m_treeItem = std::make_unique<TreeItem>();
	m_treeItem->m_Item = std::make_unique<Item>(_item);
}

template<typename Item>
typename TreeModel<Item>::TreeModel& TreeModel<Item>::operator=(const TreeModel& other)
{
	if (this != &other)
	{
		// Release existing resources
		m_treeItem.reset();

		m_nullItemPtr = std::make_unique<Item>();

		// Perform a deep copy of the tree
		if (other.m_treeItem)
		{
			m_treeItem = std::make_unique<TreeItem>();
			copyRecursive(m_treeItem, other.m_treeItem);
		}
	}
	return *this;
}

template<typename Item>
typename bool TreeModel<Item>::IsNull() const
{
	if (!m_treeItem || !m_treeItem->m_Item)
		return true;
	return false;
}

template<typename Item>
void TreeModel<Item>::copyRecursive(std::unique_ptr<TreeItem>& dest, const std::unique_ptr<TreeItem>& src)
{
	if (src)
	{
		dest->m_Item = std::make_unique<Item>(*src->m_Item);

		for (const auto& child : src->m_Children)
		{
			auto newChild = std::make_unique<TreeItem>();
			copyRecursive(newChild, child);
			dest->m_Children.push_back(std::move(newChild));
		}
	}
}

template<typename Item>
void TreeModel<Item>::from_json_recursive(std::unique_ptr<TreeItem>& treeItem, const json& j)
{
	if (!j.is_null() && j.find("item") != j.end())
	{
		// Deserialize the item
		Item item = j["item"].get<Item>();
		treeItem->m_Item = std::make_unique<Item>(std::move(item));

		// Deserialize the children if they exist
		if (j.find("children") != j.end())
		{
			for (const auto& child_json : j["children"])
			{
				auto childTreeItem = std::make_unique<TreeItem>();
				from_json_recursive(childTreeItem, child_json);
				treeItem->m_Children.push_back(std::move(childTreeItem));
			}
		}
	}
}

/**
@brief Appends a new item to the tree.

This comment is to long, because I want to test doxygen!

@param _item		The item to be added.
@param _parentItem	The parent item under which the new item will be added.

@return
- @c true  The item was successfully added to the tree.
- @c false The item could not be added, possibly because the parent item was not found in the tree structure.

@details
If the parent item is found in the tree structure, the new item will be added as a child
of the parent item. If the parent item is not found, the method will attempt to find the
parent item recursively in the children of all existing items.

@note
doxygen text
@code if (m_treeItem.m_Item == _parentItem) @endcode

@warning
doxygen text
@see
doxygen text
doxygen text
@todo
doxygen text
doxygen text
@throws
doxygen text
*/
template<typename Item>
bool TreeModel<Item>::Append(Item _item, Item _parentItem, const bool after)
{
	bool bAdded = false;

	if (*m_treeItem->m_Item == _parentItem)
	{
		auto treeItem = std::make_unique<TreeItem>();
		treeItem->m_Item = std::make_unique<Item>(_item);
		if (after)
			m_treeItem->m_Children.emplace_back(std::move(treeItem));
		else
			m_treeItem->m_Children.insert(m_treeItem->m_Children.begin(), std::move(treeItem));
		bAdded = true;
	}
	else
	{
		for (auto& item : m_treeItem->m_Children)
		{
			bAdded = addRecursive(item, _item, _parentItem, after);
			if (bAdded)
				break;
		}
	}

	return bAdded;
}

/**
@note Start is 0
*/
template<typename Item>
Item TreeModel<Item>::Get(const size_t _index)
{
	Item result;

	size_t _nCounter = -1;
	if (!getRecursive(m_treeItem, result, _index, _nCounter))
		throw new std::exception("Index not found!");
	return result;
}

/**
Sort second column from small to big.
*/
template<class Item>
void TreeModel<Item>::SortSecondColumn()
{
	if (m_treeItem->m_Children.size() < 1)
		return;

	auto& children = m_treeItem->m_Children;
	std::sort(children.begin(), children.end(), [](const std::unique_ptr<TreeItem>& a, const std::unique_ptr< TreeItem>& b)
		{
			return *a < *b;
		});
}


/**
Sort second column from small to big.
*/
template<class Item>
void TreeModel<Item>::SortAllColumns()
{
	sortColumnsRecursive(m_treeItem);
}

template<class Item>
void TreeModel<Item>::sortColumnsRecursive(std::unique_ptr<TreeItem>& treeItem)
{
	if (treeItem->m_Children.size() < 1)
		return;

	auto& children = treeItem->m_Children;
	std::sort(children.begin(), children.end(), [](const std::unique_ptr<TreeItem>& a, const std::unique_ptr< TreeItem>& b)
		{
			return *a < *b;
		});

	std::for_each(children.begin(), children.end(), [this](std::unique_ptr<TreeItem>& ti)
		{
			sortColumnsRecursive(ti);
		});
}

template<class Item>
size_t TreeModel<Item>::Size() const
{
	if (IsNull())
		return 0;

	size_t nResult = 1;

	for (auto& treeItem : m_treeItem->m_Children)
	{
		nResult += sizeRecursive(treeItem, false);
	}

	return nResult;
}

template<class Item>
size_t TreeModel<Item>::SizeLeaveNodes() const
{
	if (IsNull())
		return 0;

	size_t nResult = 0;

	for (auto& treeItem : m_treeItem->m_Children)
	{
		nResult += sizeRecursive(treeItem, true);
	}

	return nResult;
}

template<typename Item>
bool TreeModel<Item>::addRecursive(std::unique_ptr<TreeItem>& _treeItem, Item _item, Item _parentItem, bool after)
{
	if (*_treeItem->m_Item == _parentItem)
	{
		auto elems = std::make_unique<TreeItem>();
		elems->m_Item = std::make_unique<Item>(_item);
		if (after)
			_treeItem->m_Children.emplace_back(std::move(elems));
		else
			_treeItem->m_Children.insert(_treeItem->m_Children.begin(), std::move(elems));
		return true;
	}

	for (auto& layerChilds : _treeItem->m_Children)
	{
		if (addRecursive(layerChilds, _item, _parentItem, after))
			return true;
	}
	return false;
}

template<typename Item>
size_t TreeModel<Item>::sizeRecursive(const std::unique_ptr<TreeItem>& _treeItem, bool _countJustLeaveNodes) const
{
	size_t _nCounter = 0;

	if (!_countJustLeaveNodes || (_countJustLeaveNodes && _treeItem->IsLeaveNode()))
		_nCounter = 1;

	for (auto& treeItem : _treeItem->m_Children)
	{
		_nCounter += sizeRecursive(treeItem, _countJustLeaveNodes);
	}
	return _nCounter;
};

template<typename Item>
bool TreeModel<Item>::getRecursive(std::unique_ptr<TreeItem>& _treeItem, Item& _refItem, const size_t _index, size_t& _nCounter)
{
	_nCounter++;

	if (_index == _nCounter)
	{
		_refItem = *_treeItem->m_Item;
		return true;
	}

	for (auto& item : _treeItem->m_Children)
	{
		if (getRecursive(item, _refItem, _index, _nCounter))
		{
			return true;
		}
	}

	return false;
};

/**
@note Start is 0
*/
template <class Item>
std::vector<Item> TreeModel<Item>::GetPath(const size_t index)
{
	std::vector<Item> path;
	if (!m_treeItem || index >= Size())
	{
		// Return an empty path if the tree is empty or the index is out of bounds
		return path;
	}

	size_t currentIndex = 0;
	getRecursivePath(m_treeItem, index, currentIndex, path);
	return path;
}

template <class Item>
bool TreeModel<Item>::getRecursivePath(std::unique_ptr<TreeItem>& currentItem, const size_t targetIndex, size_t& currentIndex, std::vector<Item>& path)
{
	path.push_back(*(currentItem->m_Item));

	if (currentIndex == targetIndex)
	{
		// Found the target item, path is complete
		return true;
	}

	for (auto& child : currentItem->m_Children)
	{
		currentIndex++;
		// Recursively explore the child's subtree
		if (getRecursivePath(child, targetIndex, currentIndex, path))
			return true;
	}

	// If we reach here, the current item is not on the path to the target item
	// Remove it from the path
	path.pop_back();
	return false;
}

template<typename Item>
json TreeModel<Item>::GetJson() const
{
	// Helper lambda function to recursively convert TreeItems to JSON
	std::function<json(const std::unique_ptr<TreeItem>&)> treeItemToJson = [&](const std::unique_ptr<TreeItem>& treeItem) -> json
	{
		if (!treeItem || !treeItem->m_Item)
		{
			// If the TreeItem or its Item is null, return a null JSON object
			return json{};
		}

		json itemJson;
		itemJson["item"] = *treeItem->m_Item;// Assuming that the Item type can be serialized to JSON

		if (!treeItem->IsLeaveNode())
		{
			for (const auto& child : treeItem->m_Children)
			{
				itemJson["children"].push_back(treeItemToJson(child));
			}
		}

		return itemJson;
	};

	// Assuming that the tree model itself is not null, serialize the root item and its descendants
	if (!IsNull())
	{
		return treeItemToJson(m_treeItem);
	}

	// If the tree model is null, return an empty JSON object
	return json{};
}

template<typename Item>
std::unique_ptr<Item>& TreeModel<Item>::Get(std::vector<Item> structure)
{
	if (!m_treeItem || !m_treeItem->m_Item || structure.size() < 1 || *m_treeItem->m_Item != structure[0])
	{
		return m_nullItemPtr;
	}

	auto currentNode = m_treeItem.get();

	// Traverse the tree based on the structure vector
	for (size_t i = 1; i < structure.size(); ++i)
	{
		// Find the child with the current structure item
		auto it = std::find_if(currentNode->m_Children.begin(), currentNode->m_Children.end(),
			[&structure, i](const auto& child)
			{
				return child->m_Item && *(child->m_Item) == structure[i];
			});

		if (it == currentNode->m_Children.end())
		{
			return m_nullItemPtr;
		}

		// Move to the next level in the tree
		currentNode = it->get();
	}

	// Return the found item
	return currentNode->m_Item;
};
#pragma endregion
#pragma region Iterators
/**
Iterator base class.
@see Literature
Design Patterns: Elements of Reusable Object-Oriented Software
Chapter: BehaviourPatterns / Iterator
*/
template <class Item>
class Iterator
{
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() = 0;
	virtual Item CurrentItem() = 0;
protected:
	Iterator();
};

template<class Item>
inline Iterator<Item>::Iterator()
{
}

/**
Iterates through all elements from top to bottom.
*/
template <class Item>
class TopDownIterator : public Iterator<Item>
{
protected:
	struct IteratorOutOfBounds : std::exception
	{
		using std::exception::exception;
	};
public:
	TopDownIterator(TreeModel<Item>);
	virtual void First();
	virtual void Next();
	virtual bool IsDone();
	virtual Item CurrentItem();
protected:
	size_t m_lCurrent = 0;
	TreeModel<Item> m_tree;
};

/**
Iterates through tree structure from to to bottom.
*/
template<typename Item>
TopDownIterator<Item>::TopDownIterator(TreeModel<Item> tree) : m_tree(tree)
{
}

template<typename Item>
void TopDownIterator<Item>::First()
{
	m_lCurrent = 0;
}

template<typename Item>
void TopDownIterator<Item>::Next()
{
	m_lCurrent++;
}

template<typename Item>
bool TopDownIterator<Item>::IsDone()
{
	return m_lCurrent >= m_tree.Size();
}

template<typename Item>
Item TopDownIterator<Item>::CurrentItem()
{
	//if (IsDone())
	//{
	//	throw IteratorOutOfBounds("No valid state!");
	//}

	return m_tree.Get(m_lCurrent);
}
#pragma endregion
}
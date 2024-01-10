#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <exception>
#include <memory>

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
		bool IsLeaveNode() { return m_Children.size() == 0; };
		bool operator<(const TreeItem& other) const { return (*this->m_Item < *other.m_Item); };
	};
public:
	TreeModel() = default;
	~TreeModel() = default;
	TreeModel(const TreeModel& other)
	{
		// Perform a deep copy of the tree
		if (other.m_treeItem)
		{
			m_treeItem = std::make_unique<TreeItem>();
			copyRecursive(m_treeItem, other.m_treeItem);
		}
	}

	// Copy Assignment Constructor
	TreeModel& operator=(const TreeModel& other)
	{
		if (this != &other)
		{
			// Release existing resources
			m_treeItem.reset();

			// Perform a deep copy of the tree
			if (other.m_treeItem)
			{
				m_treeItem = std::make_unique<TreeItem>();
				copyRecursive(m_treeItem, other.m_treeItem);
			}
		}
		return *this;
	}

	/**
	Create the tree model.
	@param _item	root item
	*/
	TreeModel(Item _item)
	{
		m_treeItem = std::make_unique<TreeItem>();
		m_treeItem->m_Item = std::make_unique<Item>(_item);
	};
	bool Append(Item, Item);
	Item Get(const size_t);
	std::unique_ptr<TreeItem>& GetTreeItem()
	{
		return m_treeItem;
	};

	size_t Size() const;
	/**
	Sort second column from small to big.
	*/
	void SortSecondColumn();
	void SortAllColumns();
	void Clear()
	{
		m_treeItem.reset();
	};
	bool IsNull() const
	{
		if (!m_treeItem || !m_treeItem->m_Item)
			return true;
		return false;
	};
private:
	bool addRecursive(std::unique_ptr<TreeItem>&, Item, Item);
	bool getRecursive(std::unique_ptr<TreeItem>&, Item&, const size_t, size_t&);
	size_t sizeRecursive(const std::unique_ptr<TreeItem>&) const;
	void sortColumnsRecursive(std::unique_ptr<TreeItem>&);
	void copyRecursive(std::unique_ptr<TreeItem>& dest, const std::unique_ptr<TreeItem>& src)
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
	};
private:
	std::unique_ptr<TreeItem> m_treeItem;
};

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
bool TreeModel<Item>::Append(Item _item, Item _parentItem)
{
	bool bAdded = false;

	if (*m_treeItem->m_Item == _parentItem)
	{
		auto treeItem = std::make_unique<TreeItem>();
		treeItem->m_Item = std::make_unique<Item>(_item);
		m_treeItem->m_Children.emplace_back(std::move(treeItem));
		bAdded= true;
	}
	else
	{
		for (auto& item : m_treeItem->m_Children)
		{
			bAdded = addRecursive(item, _item, _parentItem);
			if (bAdded)
				break;
		}
	}

	return bAdded;
}

template<typename Item>
Item TreeModel<Item>::Get(const size_t _index)
{
	Item result;

	size_t _nCounter = -1;
	if(!getRecursive(m_treeItem, result, _index, _nCounter))
		throw new std::exception("Index not found!");
	return result;
}

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
void TreeModel<Item>::sortColumnsRecursive(std::unique_ptr<TreeItem> &treeItem)
{
	if(treeItem->m_Children.size() < 1)
		return;

	auto& children = treeItem->m_Children;
	std::sort(children.begin(), children.end(), [](const std::unique_ptr<TreeItem> &a, const std::unique_ptr< TreeItem> &b)
		{
			return *a < *b;
		});

	std::for_each(children.begin(), children.end(), [this](std::unique_ptr<TreeItem> &ti) { sortColumnsRecursive(ti); });
}

template<class Item>
size_t TreeModel<Item>::Size() const
{
	if (IsNull())
		return 0;

	size_t nResult= 1;

	for (auto& treeItem : m_treeItem->m_Children)
	{
		nResult+= sizeRecursive(treeItem);
	}

	return nResult;
}

template<typename Item>
bool TreeModel<Item>::addRecursive(std::unique_ptr<TreeItem>& _treeItem, Item _item, Item _parentItem)
{
	if (*_treeItem->m_Item == _parentItem)
	{
		auto elems = std::make_unique<TreeItem>();
		elems->m_Item = std::make_unique<Item>(_item);
		_treeItem->m_Children.emplace_back(std::move(elems));
		return true;
	}

	for (auto& layerChilds : _treeItem->m_Children)
	{
		if (addRecursive(layerChilds, _item, _parentItem))
			return true;
	}
	return false;
}

template<typename Item>
size_t TreeModel<Item>::sizeRecursive(const std::unique_ptr<TreeItem>& _treeItem) const
{
	size_t _nCounter= 1;
	for (auto& treeItem : _treeItem->m_Children)
	{
		_nCounter+= sizeRecursive(treeItem);
	}
	return _nCounter;
};

template<typename Item>
bool TreeModel<Item>::getRecursive(std::unique_ptr<TreeItem> &_treeItem, Item &_refItem, const size_t _index, size_t& _nCounter)
{
	_nCounter++;

	if(_index == _nCounter)
	{
		_refItem = *_treeItem->m_Item;
		return true;
	}

	for(auto& item : _treeItem->m_Children)
	{
		if(getRecursive(item, _refItem, _index, _nCounter))
		{
			return true;
		}
	}

	return false;
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
class Iterator {
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
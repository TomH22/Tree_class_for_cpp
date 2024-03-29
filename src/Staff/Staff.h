#pragma once
#include "Tree.h"

#include <memory>
#include <string>

struct Staff {
	enum class RANK : int
	{
		PRIVATE = 0,
		CORPORAL = 1,
		SERGANT = 2,
		STAFF_SERGANT = 3,
	};
private:
	std::wstring name = L"";
	RANK rank = RANK::PRIVATE;
public:
	Staff() = default;
	//Staff(const Staff& other) : name(other.name), rank(other.rank) {}
	//bool operator=(const Staff& other) {};
	Staff(std::wstring _name, RANK _rank) : name(_name), rank(_rank) {};
	Staff(std::string _name, RANK _rank);
	bool operator==(const Staff other) const;
	bool operator!=(const Staff other) const { return !(*this == other); };
	bool operator<(const Staff& other) const;
	bool operator>=(const Staff& other) const { return !(*this < other); }
	std::wstring GetName() const { return name; };
	std::string GetNameAsString() const;
	std::shared_ptr<char> GetNameAsSpChar() const;
	RANK GetRank() const { return rank; };
	std::wstring GetRankLine();
	void SetRank(RANK);
	bool IsNull() const { return name.empty(); };
};

//namespace missing ...

template <typename BasicJsonType>
static void to_json(BasicJsonType& j, const Staff& staff)
{
	if (staff.IsNull())
		j = json{};
	else
		j = json{
			{
			"staff",
			{
				{"name", staff.GetNameAsString()},
				{"rank", static_cast<int>(staff.GetRank())}
			}
			}
	};
}

template<typename BasicJsonType>
static void from_json(const BasicJsonType& j, Staff& staff)
{
	if (j.is_null() || j.find("staff") == j.end())
	{
		// nothing
	}
	else
	{
		auto obj = j["staff"];
		staff = Staff(obj["name"].get<std::string>(), static_cast<Staff::RANK>(obj["rank"].get<int>()));
	}
}


//////////////////////////////////////////////////////////////////////
// Iterator for staff tree model.
class StaffTopDownIterator : public henn::TopDownIterator<Staff>
{
public:
	//////////////////////////////////////////////////////////////////
	// Constructor of class
	// @pram
	//	_henn::TreeModel		data
	//	_bOrderAlphabetical		order columns alphabetical
	//	_bShowStaffSergants		show staff sergants
	//	_bShowSergants			show sergants
	//	_bShowCorporals			show corporals
	//	_bShowPrivates			show privates
	StaffTopDownIterator(henn::TreeModel<Staff> TreeModel, bool _bOrderAlphabetical, bool _bShowStaffSergants, bool _bShowSergants, bool _bShowCorporals, bool _bShowPrivates);
	virtual void First() override;
	virtual void Next() override;
	virtual bool IsDone() override;
private:
	bool show(size_t);
private:
	bool m_bOrderAlphabetical = false;
	bool m_bShowStaffSergants = true;
	bool m_bShowSergants = true;
	bool m_bShowCorporals = true;
	bool m_bShowPrivates = true;
	Staff staffTmp;
};

//////////////////////////////////////////////////////////////////////
// Alternative iterator for staff tree model.
class StaffTopDownIterator2 : public henn::TopDownIterator<Staff>
{
public:
	//////////////////////////////////////////////////////////////////
	// Constructor of class
	// @pram
	//	_henn::TreeModel		data
	//	_bOrderSecondColumn		order second column (bellow root) alphabetical
	//	_bSecondOutput1Column	output staff of rank STAFF_SERGANT twice
	//	_bSecondOutput2Column	output staff of rank SERGANT twice
	StaffTopDownIterator2(henn::TreeModel<Staff> TreeModel, bool _bOrderSecondColumn = false, bool _bSecondOutput1Column = false, bool _bSecondOutput2Column = false);;
	virtual void Next() override;
	virtual Staff CurrentItem() override;;
private:
	bool m_bSecondOutput1Column;
	bool m_bSecondOutput2Column;
	bool m_bSecondOutput = false;
	Staff staffTmp;
};


//////////////////////////////////////////////////////////////////////
// Iterator to output top item of tree model.
class StaffIteratorTopItem : public henn::TopDownIterator<Staff>
{
public:
	StaffIteratorTopItem(henn::TreeModel<Staff> TreeModel) : TopDownIterator(TreeModel) {};
	virtual bool IsDone() override { return TopDownIterator::IsDone() || m_lCurrent > 0; };
};
#pragma once
#include <string>
#include <vector>

class Good
{
	int id;
	std::string type, model, manufacturer;
public:
	// Constructors
	Good();
	Good(int id, std::string type, std::string model, std::string manufacturer);

	// Setters
	void setId(int id);
	void setType(std::string type);
	void setModel(std::string model);
	void setManufacturer(std::string manufacturer);

	// Getters
	int getId();
	std::string getType();
	std::string getModel();
	std::string getManufacturer();
	bool operator < (const Good &other) const;

	// Serialization method
	template<class Archive>
	void serialize(Archive &archive);
};
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

template<typename ValueType>
class GoodIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
	ValueType* p;
public:
	GoodIterator(ValueType* p);
	GoodIterator(typename std::vector<ValueType>::iterator it);

	bool operator==(GoodIterator<ValueType> const &other) const;
	bool operator!=(GoodIterator<ValueType> const &other) const;
	ValueType operator*() const;
	ValueType* const operator->() const;
	GoodIterator &operator++();
};

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

class GoodContainer
{
	std::vector<Good> goods;
public:
	typedef GoodIterator<Good> iterator;
	iterator begin();
	iterator end();

	std::vector<Good> getGoods();

	GoodContainer* add(Good good);
	GoodContainer* drop_by_id(int id);
	GoodContainer* clear();
	Good find_by_id(int id);
	bool is_id_unique(int id);
	std::vector<Good> sortedGoods();

	void save();
	void load();
};
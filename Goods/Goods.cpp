// Goods.cpp : Defines the exported functions for the DLL application.
//
#pragma once
#include "stdafx.h"
#include "Goods.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"

//----------------------------------------------------------------------------------
//---------------------------- GOODS   CLASS ---------------------------------------
//----------------------------------------------------------------------------------

// Constructors
Good::Good() {};
Good::Good(int id, std::string type, std::string model, std::string manufacturer) {
	this->id = id;
	this->type = type;
	this->model = model;
	this->manufacturer = manufacturer;
};

// Setters
void Good::setId(int id) { this->id = id; };
void Good::setType(std::string type) { this->type = type; };
void Good::setModel(std::string model) { this->model = model; };
void Good::setManufacturer(std::string manufacturer) { this->manufacturer = manufacturer; };

// Getters
int Good::getId() { return id; };
std::string Good::getType() { return type; };
std::string Good::getModel() { return model; };
std::string Good::getManufacturer() { return manufacturer; };

bool Good::operator < (const Good &other) const
{
	return id < other.id;
}
// Serialiation method
template<class Archive>
void Good::serialize(Archive &archive) {
	archive(id, type, model, manufacturer);
}
//----------------------------------------------------------------------------------------------
//------------------------------------  GOOD ITERATOR CLASS  -----------------------------------
//----------------------------------------------------------------------------------------------

template<typename ValueType>
GoodIterator<ValueType>::GoodIterator(ValueType* p) : p(p) {};

template<typename ValueType>
GoodIterator<ValueType>::GoodIterator(typename std::vector<ValueType>::iterator it) : p(it._Ptr) {};

template<typename ValueType>
bool GoodIterator<ValueType>::operator==(GoodIterator<ValueType> const &other) const { return p == other.p; };

template<typename ValueType>
bool GoodIterator<ValueType>::operator!=(GoodIterator<ValueType> const &other) const { return p != other.p; };

template<typename ValueType>
ValueType GoodIterator<ValueType>::operator*() const { return *p; };

template<typename ValueType>
ValueType* const GoodIterator<ValueType>::operator->() const { return &(*p); };

template<typename ValueType>
GoodIterator<ValueType> &GoodIterator<ValueType>::operator++() {
	++p;
	return *this;
};


//-------------------------------------------------------------------------------------------------
//--------------------------------- GOOD  CONTAINER CLASS -----------------------------------------
//-------------------------------------------------------------------------------------------------

GoodContainer::iterator GoodContainer::begin() { return iterator(goods.begin()); };
GoodContainer::iterator GoodContainer::end() { return iterator(goods.end()); };
std::vector<Good> GoodContainer::getGoods() { return sortedGoods(); };

GoodContainer* GoodContainer::add(Good good) {
	goods.push_back(good);
	return this;
};

GoodContainer* GoodContainer::drop_by_id(int id)
{
	int index = 0;
	for (iterator it = begin(); it != end(); ++it) {
		if (it->getId() == id) {
			goods.erase(goods.begin() + index);
			return this;
		}
		index++;
	}
	return this;
};

GoodContainer* GoodContainer::clear()
{
	goods.clear();
	return this;
}

Good GoodContainer::find_by_id(int id)
{
	for (iterator it = begin(); it != end(); ++it) {
		if (it->getId() == id) return *it;
	}
}
bool GoodContainer::is_id_unique(int id)
{
	for (iterator it = begin(); it != end(); ++it) {
		if (it->getId() == id) return false;
	}
	return true;
}

std::vector<Good> GoodContainer::sortedGoods()
{
	std::vector<Good> sorted = goods;
	std::sort(sorted.begin(), sorted.end());
	return sorted;
};

void GoodContainer::save() {
	const char* filename = "goods";
	std::ofstream out(filename, std::ios::binary);
	cereal::BinaryOutputArchive oarchive(out);

	for (GoodIterator<Good> it = begin(); it != end(); ++it) {
		oarchive(*it);
		//out.write((char*)&(*it), sizeof(Good));
	}
	out.close();
}

void GoodContainer::load() {
	const char* filename = "goods";
	std::ifstream in(filename, std::ios::binary);
	cereal::BinaryInputArchive iarchive(in);

	Good good;
	//while (in.read((char*)&good, sizeof(Good)))
	while (in) {
		try {
			iarchive(good);
			add(good);
		}
		catch (cereal::Exception) { break; }
	}
	in.close();
}
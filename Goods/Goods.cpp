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

// Конструкторы
Good::Good() {};
Good::Good(int id, std::string type, std::string model, std::string manufacturer) {
	this->id = id;
	this->type = type;
	this->model = model;
	this->manufacturer = manufacturer;
};

// Сеттеры
void Good::setId(int id) { this->id = id; };
void Good::setType(std::string type) { this->type = type; };
void Good::setModel(std::string model) { this->model = model; };
void Good::setManufacturer(std::string manufacturer) { this->manufacturer = manufacturer; };

// Геттеры
int Good::getId() { return id; };
std::string Good::getType() { return type; };
std::string Good::getModel() { return model; };
std::string Good::getManufacturer() { return manufacturer; };

bool Good::operator < (const Good &other) const
{
	return id < other.id;
}
// Метод сериализации
template<class Archive>
void Good::serialize(Archive &archive) {
	archive(id, type, model, manufacturer);
}
//----------------------------------------------------------------------------------------------
//------------------------------------  GOOD ITERATOR CLASS  -----------------------------------
//----------------------------------------------------------------------------------------------

template<typename ValueType>
GoodIterator<ValueType>::GoodIterator(ValueType* p) : p(p) {};

// Конструктор копирования
template<typename ValueType>
GoodIterator<ValueType>::GoodIterator(typename std::vector<ValueType>::iterator it) : p(it._Ptr) {};

// Оператор логического равенства двух итераторов
template<typename ValueType>
bool GoodIterator<ValueType>::operator==(GoodIterator<ValueType> const &other) const { return p == other.p; };

// Оператор неравенства двух итераторов
template<typename ValueType>
bool GoodIterator<ValueType>::operator!=(GoodIterator<ValueType> const &other) const { return p != other.p; };


// Доступ к объекту
template<typename ValueType>
ValueType GoodIterator<ValueType>::operator*() const { return *p; };

// Доступ к членам объекта
template<typename ValueType>
ValueType* const GoodIterator<ValueType>::operator->() const { return &(*p); };


// Перемещение итератора
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

// Добавление элемента в конец коллекции
GoodContainer* GoodContainer::add(Good good) {
	goods.push_back(good);
	return this;
};

// Удаление элемента коллекции по значению ID
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

// Очистка контейнера
GoodContainer* GoodContainer::clear()
{
	goods.clear();
	return this;
}

// Поиск элемента коллекции по ID
Good GoodContainer::find_by_id(int id)
{
	for (iterator it = begin(); it != end(); ++it) {
		if (it->getId() == id) return *it;
	}
}

// Проверка, существует ли указанный ID в коллекции
bool GoodContainer::is_id_unique(int id)
{
	for (iterator it = begin(); it != end(); ++it) {
		if (it->getId() == id) return false;
	}
	return true;
}

// Сортировка коллекции по ID
std::vector<Good> GoodContainer::sortedGoods()
{
	std::vector<Good> sorted = goods;
	std::sort(sorted.begin(), sorted.end());
	return sorted;
};

// Сохранение коллекции в файл
void GoodContainer::save() {
	const char* filename = "goods";
	std::ofstream out(filename, std::ios::binary);
	// Создание архива для сериализации
	cereal::BinaryOutputArchive oarchive(out);

	for (GoodIterator<Good> it = begin(); it != end(); ++it) {
		oarchive(*it);
	}
	out.close();
}

// Загрузка коллекции из файла 
void GoodContainer::load() {
	const char* filename = "goods";
	std::ifstream in(filename, std::ios::binary);
	// Создания архива для десериализаци
	cereal::BinaryInputArchive iarchive(in);

	Good good;
	while (in) {
		try {
			iarchive(good);
			add(good);
		}
		catch (cereal::Exception) { break; }
	}
	in.close();
}
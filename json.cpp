#include <iostream>
#include <string>
#include <limits>
#include <assert.h>
#include <fstream>

#include "json.hpp"

struct json::impl{
	enum class tipo{
		null,
		number,
		boolean,
		string,
		list,
		dictionary
	}tipo;
	
	double num;
	bool boolean;
	std::string str;
	
	struct lista{
		json valore;
		lista* next;
	};
	
	lista* l_front;
	lista* l_back;
	
	struct dizionario{
		std::pair<std::string,json> diz;
		dizionario* next;
	};
	
	dizionario* d_front;
	dizionario* d_back;
	
	impl(){
		tipo = tipo::null;
		
		l_front = nullptr;
		l_back = nullptr;
		d_front = nullptr;
		d_back = nullptr;
	}
};

//ITERATORE LISTA
struct json::list_iterator{
	//valore restituito dall'iteratore
	using value_type = json;
	//definisce il tipo dell'oggetto cui si fa riferimento
	using reference = json&;
	//definisce il tipo di puntatore
	using pointer = json*;
	//indica che l'iteratore supporta solo l'operazione di avanzamento in avanti e non supporta l'accesso casuale ai suoi elementi
	using iterator_category = std::forward_iterator_tag;
			
	list_iterator(impl::lista* ptr) : m_ptr(ptr){}
	
	//ritorno una reference		
	reference operator*() const{return m_ptr->valore;}
	
	//ritorno un puntatore		
	pointer operator->() const{return &(m_ptr->valore);}
	
	//operatore di postincremento		
	list_iterator& operator++(){
		m_ptr = m_ptr->next;
		return *this;
	}
	
	//operatore di preincremento			
	list_iterator operator++(int){
		list_iterator temp(m_ptr);
		m_ptr = m_ptr->next;
		return temp;
	}
			
	bool operator==(list_iterator const& rhs) const{
		return m_ptr == rhs.m_ptr;
	}
			
	bool operator!=(list_iterator const& rhs) const{
		return m_ptr != rhs.m_ptr;
	}
						
	private:
		impl::lista* m_ptr;
};
		
json::list_iterator json::begin_list(){
	if(this->is_list())
		return list_iterator(pimpl->l_front);
	else{
		throw json_exception{"Impossibile usare la funzione begin_list(). L'oggetto JSON non è di tipo lista"};
	}
}
			
json::list_iterator json::end_list(){
	if(this->is_list())
		return list_iterator(nullptr);
	else{
		throw json_exception{"Impossibile usare la funzione end_list(). L'oggetto JSON non è di tipo lista"};	
	}
}

//ITERATORE DIZIONARIO	
struct json::dictionary_iterator{
	using value_type = std::pair<std::string,json>;
	using reference = std::pair<std::string,json>&;
	using pointer = std::pair<std::string,json>*;
	using iterator_category = std::forward_iterator_tag;
			
	dictionary_iterator(impl::dizionario* ptr) : m_ptr(ptr){}
			
	reference operator*() const{return m_ptr->diz;}
			
	pointer operator->() const{return &(m_ptr->diz);}
			
	dictionary_iterator& operator++(){
		m_ptr = m_ptr->next;
		return *this;
	}
				
	dictionary_iterator operator++(int){
		dictionary_iterator temp(m_ptr);
		m_ptr = m_ptr->next;
		return temp;
	}
			
	bool operator==(dictionary_iterator const& rhs) const{
		return m_ptr == rhs.m_ptr;
	}
			
	bool operator!=(dictionary_iterator const& rhs) const{
		return m_ptr != rhs.m_ptr;
	}
						 
	private:
		impl::dizionario* m_ptr;
};
		
json::dictionary_iterator json::begin_dictionary(){
	if(this->is_dictionary())
		return dictionary_iterator(pimpl->d_front);
	else{
		throw json_exception{"Impossibile usare la funzione begin_dictionary(). L'oggetto JSON non è di tipo dizionario"};
	}
}
			
json::dictionary_iterator json::end_dictionary(){
	if(this->is_dictionary())
		return dictionary_iterator(nullptr);
	else{
		throw json_exception{"Impossibile usare la funzione begin_dictionary(). L'oggetto JSON non è di tipo dizionario"};
	}
}
		
//ITERATORE CONST LISTA
struct json::const_list_iterator{
	using value_type = json const;
	using reference = json const&;
	using pointer = json const*;
	using iterator_category = std::forward_iterator_tag;
			
	const_list_iterator(impl::lista* ptr) : m_ptr(ptr){}
			
	reference operator*() const{return m_ptr->valore;}
			
	pointer operator->() const{return &(m_ptr->valore);}
			
	const_list_iterator& operator++(){
		m_ptr = m_ptr->next;
		return *this;
	}
				
	const_list_iterator operator++(int){
		const_list_iterator temp(m_ptr);
		m_ptr = m_ptr->next;
		return temp;
	}
			
	bool operator==(const_list_iterator const& rhs) const{
		return m_ptr == rhs.m_ptr;
	}
			
	bool operator!=(const_list_iterator const& rhs) const{
		return m_ptr != rhs.m_ptr;
	}
						
	private:
		impl::lista* m_ptr;
};
		
json::const_list_iterator json::begin_list() const{
	if(this->is_list())
		return const_list_iterator(pimpl->l_front);
	else{
		throw json_exception{"Impossibile usare la funzione begin_list() const. L'oggetto JSON non è di tipo lista"};	
	}
}
			
json::const_list_iterator json::end_list() const{
	if(this->is_list())
		return const_list_iterator(nullptr);
	else{
		throw json_exception{"Impossibile usare la funzione end_list() const. L'oggetto JSON non è di tipo lista"};
	}
}
		
//ITERATORE CONST DIZIONARIO
struct json::const_dictionary_iterator{
	using value_type = std::pair<std::string,json> const;
	using reference = std::pair<std::string,json> const&;
	using pointer = std::pair<std::string,json> const*;
	using iterator_category = std::forward_iterator_tag;
			
	const_dictionary_iterator(impl::dizionario* ptr) : m_ptr(ptr){}
			
	reference operator*() const{return m_ptr->diz;}
			
	pointer operator->() const{return &(m_ptr->diz);}
			
	const_dictionary_iterator& operator++(){
		m_ptr = m_ptr->next;
		return *this;
	}
				
	const_dictionary_iterator operator++(int){
		const_dictionary_iterator temp(m_ptr);
		m_ptr = m_ptr->next;
		return temp;
	}
			
	bool operator==(const_dictionary_iterator const& rhs) const{
		return m_ptr == rhs.m_ptr;
	}
			
	bool operator!=(const_dictionary_iterator const& rhs) const{
		return m_ptr != rhs.m_ptr;
	}
						 
	private:
		impl::dizionario* m_ptr;
};
		
json::const_dictionary_iterator json::begin_dictionary() const{
	if(this->is_dictionary())
		return const_dictionary_iterator(pimpl->d_front);
	else{
		throw json_exception{"Impossibile usare la funzione begin_dictionary() const. L'oggetto JSON non è di tipo dizionario"};
	}		
}
			
json::const_dictionary_iterator json::end_dictionary() const{
	if(this->is_dictionary())
		return const_dictionary_iterator(nullptr);
	else{
		throw json_exception{"Impossibile usare la funzione end_dictionary() const. L'oggetto JSON non è di tipo dizionario"};
	}
}

//COSTRUTTORI E DISTRUTTORI DELLA CLASSE
//costruttore di default
json::json(){
	this->pimpl = new impl();
	this->pimpl->tipo = impl::tipo::null;
}

//copy constructor
json::json(json const& rhs){
	if (rhs.pimpl == nullptr){
		this->pimpl = nullptr;
		return;
	}

	this->pimpl = new impl();
	this->pimpl->tipo = rhs.pimpl->tipo;

	if(rhs.is_number()){
		this->pimpl->num = rhs.pimpl->num;
	}else if(rhs.is_bool()){
		this->pimpl->boolean = rhs.pimpl->boolean;
	}else if(rhs.is_string()){
		this->pimpl->str = rhs.pimpl->str;
	}else if(rhs.is_list()){
		this->pimpl->l_front = nullptr;
		this->pimpl->l_back = nullptr;

		for(json::const_list_iterator it = rhs.begin_list(); it != rhs.end_list(); it++){
			impl::lista* newNode = new impl::lista;
			newNode->valore = json(*it);
			newNode->next = nullptr;

			if(this->pimpl->l_front == nullptr){
				this->pimpl->l_front = newNode;
				this->pimpl->l_back = newNode;
			}else{
				this->pimpl->l_back->next = newNode;
				this->pimpl->l_back = this->pimpl->l_back->next;
			}
		}
	}else if(rhs.is_dictionary()){
		this->pimpl->d_front = nullptr;
		this->pimpl->d_back = nullptr;

		for(json::const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary(); it++){
			impl::dizionario* newNode = new impl::dizionario;
			newNode->diz.first = it->first;
			newNode->diz.second = json(it->second);
			newNode->next = nullptr;

			if(this->pimpl->d_front == nullptr){
				this->pimpl->d_front = newNode;
				this->pimpl->d_back = newNode;
			}else{
				this->pimpl->d_back->next = newNode;
				this->pimpl->d_back = this->pimpl->d_back->next;
			}
		}
	}
}

//move semantic
json::json(json&& rhs){
	this->pimpl = rhs.pimpl;
	rhs.pimpl = nullptr;
}

//distruttore
json::~json(){
    if (pimpl == nullptr) {
        return;
    }
    else{
		if(this->is_list()){
			json::impl::lista* tmp = pimpl->l_front;
            while(tmp != nullptr){ //scorro tutta la lista
				json::impl::lista* nextNode = tmp->next;
				tmp->valore.~json(); //richiamo il distruttore per ogni elemento json contenuto nella lista
				delete tmp;
				tmp = nextNode;
			}
		}else if(this->is_dictionary()){
			json::impl::dizionario* tmp = pimpl->d_front;
			while(tmp != nullptr){ //scorro tutti i dizionari
				json::impl::dizionario* nextNode = tmp->next;
				tmp->diz.second.~json(); //richiamo il distruttore per il secondo elemento del pair
				delete tmp;
				tmp = nextNode;
			}
		}
	}
    delete pimpl;
    pimpl = nullptr;
}

/*ESEMPIO:
 * json obj1;
 * json obj2;
 * 
 * obj1 = obj2;
*/
json& json::operator=(const json& rhs) {
	if (this == &rhs) // Evita l'autoassegnazione
		return *this;

	// Effettua la deallocazione degli elementi a seconda del tipo
	switch (pimpl->tipo) {
			break;
		case impl::tipo::null:
			break;
		case impl::tipo::number:
			break;
		case impl::tipo::boolean:
			// Nessuna deallocazione necessaria per i tipi primitivi
			break;
		case impl::tipo::string:
			break;
		case impl::tipo::list:
			// Dealloca gli elementi nella lista corrente
			while (pimpl->l_front != nullptr) {
			impl::lista* tmp = pimpl->l_front;
			pimpl->l_front = pimpl->l_front->next;
			tmp->valore.~json(); // Chiamata ricorsiva per cancellare gli elementi nella lista annidata
			delete tmp;
			}
			break;

		case impl::tipo::dictionary:
			// Dealloca gli elementi nel dizionario corrente
			while (pimpl->d_front != nullptr) {
				impl::dizionario* tmp = pimpl->d_front;
				pimpl->d_front = pimpl->d_front->next;
				tmp->diz.first.clear(); // Chiamata ricorsiva per cancellare la chiave del dizionario
				tmp->diz.second.~json(); // Chiamata ricorsiva per cancellare il valore del dizionario
				delete tmp;
			}
			break;
	}

	// Esegui la deep copy del tipo
	pimpl->tipo = rhs.pimpl->tipo;

	// Effettua la copia appropriata in base al tipo
	switch (rhs.pimpl->tipo) {
		case impl::tipo::null:
			// Nessuna deep copy necessaria
			break;

		case impl::tipo::number:
			pimpl->num = rhs.pimpl->num;
			break;

		case impl::tipo::boolean:
			pimpl->boolean = rhs.pimpl->boolean;
			break;

		case impl::tipo::string:
			new (&pimpl->str) std::string(rhs.pimpl->str); // Usa l'operatore di copia per la stringa
			break;

		case impl::tipo::list:
			// Effettua la deep copy degli elementi nella lista di rhs
			for (json::const_list_iterator it = rhs.begin_list(); it != rhs.end_list(); ++it) {
				push_back(*it); // Utilizza il metodo push_back aggiornato che esegue una deep copy
			}
			break;

		case impl::tipo::dictionary:
			// Effettua la deep copy degli elementi nel dizionario di rhs
			for (json::const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary(); ++it) {
				insert(*it); // Utilizza il metodo push_back o push_front aggiornato che esegue una deep copy
			}
			break;
	}

	return *this;
}

/*ESEMPIO:
 * json obj1;
 * 
 * obj1 = getJson(); dato temporaneo
*/
json& json::operator=(json&& rhs) {
	json tmp(std::move(*this));
	this->pimpl = rhs.pimpl;
	rhs.pimpl = nullptr;
	
	return *this;
}

//METODI IS
bool json::is_null() const{
	if(this->pimpl->tipo == impl::tipo::null)
		return true;
	return false;
}

bool json::is_number() const{
	if(this->pimpl->tipo == impl::tipo::number)
		return true;
	return false;
}

bool json::is_bool() const{
	if(this->pimpl->tipo == impl::tipo::boolean)
		return true;
	return false;
}

bool json::is_string() const{
	if(this->pimpl->tipo == impl::tipo::string)
		return true;
	return false;
}

bool json::is_list() const{
	if(this->pimpl->tipo == impl::tipo::list)
		return true;
	return false;
}

bool json::is_dictionary() const{
	if(this->pimpl->tipo == impl::tipo::dictionary)
		return true;
	return false;
}

json const& json::operator[](std::string const& stri) const {
	try{
		for (json::const_dictionary_iterator it = this->begin_dictionary(); it != this->end_dictionary(); ++it) {
			if (it->first == stri)
				return it->second;
		}
	}catch(const json_exception& ecc){
		std::cerr << ecc.msg << "\n";
	}
	
	throw json_exception{"Impossibile trovare il valore nel dizionario e creare un nuovo nodo in una funzione const"};
	return *this;
}

json& json::operator[](std::string const& stri){
	try{
		for (json::dictionary_iterator it = this->begin_dictionary(); it != this->end_dictionary(); ++it) {
			if (it->first == stri)
				return it->second;
		}
	}catch(const json_exception& ecc){
			std::cerr << ecc.msg << "\n";
			return *this;
	}

	// Il nodo non è stato trovato, quindi creiamo un nuovo nodo
	json::impl::dizionario* newNode = new json::impl::dizionario;
	newNode->diz.first = stri;
	json newJ;
	newJ.set_null();
	newNode->diz.second = newJ;
	newNode->next = nullptr;

	// Aggiungiamo il nuovo nodo alla lista dei dizionari
	if (pimpl->d_front == nullptr) {
		// La lista è vuota, quindi il nuovo nodo diventa il front
		pimpl->d_front = newNode;
		pimpl->d_back = newNode;
	} else {
		// Aggiungiamo il nuovo nodo alla fine della lista
		pimpl->d_back->next = newNode;
		pimpl->d_back = newNode;
	}

	return pimpl->d_back->diz.second;
}

//ritorno il valore double all'interno dell'oggetto json
double& json::get_number(){
	if(this->is_number())
		return pimpl->num;
	else{
		throw json_exception{"Impossibile ritornare il valore l'oggetto nella posizione desiderata.\n L'oggetto JSON non è di tipo double"};
	}
}

//ritorno il valore double(const) all'interno dell' oggetto json
double const& json::get_number() const{
	if(this->is_number())
		return pimpl->num;
	else{
		throw json_exception{"Impossibile ritornare il valore const number. L'oggetto JSON non è di tipo double"};
	}
}

//ritorno il valore bool all'interno dell' oggetto json
bool& json::get_bool(){
	if(this->is_bool())
		return pimpl->boolean;
	else{
		throw json_exception{"Impossibile ritornare il valore bool. L'oggetto JSON non è di tipo boolean"};
	}	
}

//ritorno il valore bool(const) all'interno dell' oggetto json
bool const& json::get_bool() const{
	if(this->is_bool())
		return pimpl->boolean;
	else{
		throw json_exception{"Impossibile ritornare il valore const bool. L'oggetto JSON non è di tipo boolean"};
	}	
}

//ritorno il valore string all'interno dell' oggetto json
std::string& json::get_string(){
	if(this->is_string())
		return pimpl->str;
	else{
		throw json_exception{"Impossibile ritornare il valore string. L'oggetto JSON non è di tipo string"};
	}	
}

//ritorno il valore string(const) all'interno dell' oggetto json
std::string const& json::get_string() const{
	if(this->is_string())
		return pimpl->str;
	else{
		throw json_exception{"Impossibile ritornare il valore const string. L'oggetto JSON non è di tipo string"};
	}
}

//setto l'oggetto a json a stringa
void json::set_string(std::string const& x){
	if(this->pimpl->tipo == impl::tipo::null){
		this->pimpl->str = x;
	}else if(this->pimpl->tipo == impl::tipo::number){
		this->pimpl->num = 0;
		this->pimpl->str = x;
	}else if(this->pimpl->tipo == impl::tipo::boolean){
		this->pimpl->boolean = 0;
		this->pimpl->str = x;
	}else if(this->pimpl->tipo == impl::tipo::string){
		this->pimpl->str = x;
	}else if(this->pimpl->tipo == impl::tipo::list){
		//cancello gli elementi all'interno della lista dell'oggetto json
		json::impl::lista* tmp = pimpl->l_front;
		while(tmp != nullptr){
			impl::lista *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
		this->pimpl->str = x;
	}else if(this->pimpl->tipo == impl::tipo::dictionary){
		//cancello gli elementi all'interno dei dizionari dell'oggetto json
		json::impl::dizionario* tmp = pimpl->d_front;
		while(tmp != nullptr){
			impl::dizionario *nextNode = tmp->next;
			tmp->diz.second.~json();
			delete tmp;
			tmp = nextNode;
		}
		this->pimpl->str = x;
	}
	//imposto il tipo a string
	this->pimpl->tipo = impl::tipo::string;
}

//setto l'oggetto a json a booleano
void json::set_bool(bool x){
	if(this->pimpl->tipo == impl::tipo::null){
		this->pimpl->boolean = x;
	}else if(this->pimpl->tipo == impl::tipo::number){
		this->pimpl->num = 0;
		this->pimpl->boolean = x;
	}else if(this->pimpl->tipo == impl::tipo::boolean){
		this->pimpl->boolean = x;
	}else if(this->pimpl->tipo == impl::tipo::string){
		this->pimpl->str = "0";
		this->pimpl->boolean = x;
	}else if(this->pimpl->tipo == impl::tipo::list){
		//cancello gli elementi all'interno della lista dell'oggetto json
		json::impl::lista* tmp = pimpl->l_front;
		while(tmp != nullptr){
			impl::lista *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
		this->pimpl->boolean = x;
	}else if(this->pimpl->tipo == impl::tipo::dictionary){
		//cancello gli elementi all'interno dei dizionari dell'oggetto json
		json::impl::dizionario* tmp = pimpl->d_front;
		while(tmp != nullptr){
			impl::dizionario *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
		this->pimpl->boolean = x;
	}
	//imposto il tipo a string
	this->pimpl->tipo = impl::tipo::boolean;
}

//setto l'oggetto a json ad un numero
void json::set_number(double x){
	if(this->pimpl->tipo == impl::tipo::null){
		this->pimpl->num = x;
	}else if(this->pimpl->tipo == impl::tipo::number){
		this->pimpl->num = x;
	}else if(this->pimpl->tipo == impl::tipo::boolean){
		this->pimpl->boolean = 0;
		this->pimpl->num = x;
	}else if(this->pimpl->tipo == impl::tipo::string){
		this->pimpl->str = "0";
		this->pimpl->num = x;
	}else if(this->pimpl->tipo == impl::tipo::list){
		//cancello gli elementi all'interno della lista dell'oggetto json
		json::impl::lista* tmp = pimpl->l_front;
		while(tmp != nullptr){
			impl::lista *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
		this->pimpl->num = x;
	}else if(this->pimpl->tipo == impl::tipo::dictionary){
		//cancello gli elementi all'interno dei dizionari dell'oggetto json
		json::impl::dizionario* tmp = pimpl->d_front;
		while(tmp != nullptr){
			impl::dizionario *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
		this->pimpl->num = x;
	}
	//imposto il tipo a string
	this->pimpl->tipo = impl::tipo::number;
}

//setto l'oggetto a json a null
void json::set_null(){
	if(this->pimpl->tipo == impl::tipo::number){
		this->pimpl->num = 0;
	}else if(this->pimpl->tipo == impl::tipo::boolean){
		this->pimpl->boolean = 0;
	}else if(this->pimpl->tipo == impl::tipo::string){
		this->pimpl->str = "0";
	}else if(this->pimpl->tipo == impl::tipo::list){
		//cancello gli elementi all'interno della lista dell'oggetto json
		json::impl::lista* tmp = pimpl->l_front;
		while(tmp != nullptr){
			impl::lista *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
	}else if(this->pimpl->tipo == impl::tipo::dictionary){
		//cancello gli elementi all'interno dei dizionari dell'oggetto json
		json::impl::dizionario* tmp = pimpl->d_front;
		while(tmp != nullptr){
			impl::dizionario *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
	}
	//imposto il tipo a null
	this->pimpl->tipo = impl::tipo::null;
}

//setto l'oggetto a json a lista
void json::set_list(){
	if(this->pimpl->tipo == impl::tipo::number){
		this->pimpl->num = 0;
	}else if(this->pimpl->tipo == impl::tipo::boolean){
		this->pimpl->boolean = 0;
	}else if(this->pimpl->tipo == impl::tipo::string){
		this->pimpl->str = "0";
	}else if(this->pimpl->tipo == impl::tipo::list){
		//cancello gli elementi all'interno della lista dell'oggetto json
		json::impl::lista* tmp = pimpl->l_front;
		while(tmp != nullptr){
			impl::lista *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
	}else if(this->pimpl->tipo == impl::tipo::dictionary){
		//cancello gli elementi all'interno dei dizionari dell'oggetto json
		json::impl::dizionario* tmp = pimpl->d_front;
		while(tmp != nullptr){
			impl::dizionario *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
	}
	
	this->pimpl->l_front = nullptr;
	this->pimpl->l_back = nullptr;
	//imposto il tipo a string
	this->pimpl->tipo = impl::tipo::list;
}

//setto l'oggetto a json a dizionario
void json::set_dictionary(){
	if(this->pimpl->tipo == impl::tipo::number){
		this->pimpl->num = 0;
	}else if(this->pimpl->tipo == impl::tipo::boolean){
		this->pimpl->boolean = 0;
	}else if(this->pimpl->tipo == impl::tipo::string){
		this->pimpl->str = "0";
	}else if(this->pimpl->tipo == impl::tipo::list){
		//cancello gli elementi all'interno della lista dell'oggetto json
		json::impl::lista* tmp = pimpl->l_front;
		while(tmp != nullptr){
			impl::lista *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
	}else if(this->pimpl->tipo == impl::tipo::dictionary){
		//cancello gli elementi all'interno dei dizionari dell'oggetto json
		json::impl::dizionario* tmp = pimpl->d_front;
		while(tmp != nullptr){
			impl::dizionario *nextNode = tmp->next;
			delete tmp;
			tmp = nextNode;
		}
	}
	
	this->pimpl->d_front = nullptr;
	this->pimpl->d_back = nullptr;
	//imposto il tipo a string
	this->pimpl->tipo = impl::tipo::dictionary;
}

//push_front
void json::push_front(json const& x){
	if(!this->is_list()){
		throw json_exception{"Impossibile eseguire il push_front. L'oggetto JSON non è di tipo lista"};
	}else{
		impl::lista *newNode = new impl::lista;
		newNode->valore = x;
		newNode->next = nullptr;
				
		//lista vuota
		if(pimpl->l_front == nullptr){
			pimpl->l_front = newNode;
			pimpl->l_back = newNode;
		}else{
			newNode->next = pimpl->l_front;
			pimpl->l_front = newNode;
		}
	}
}

//push_back
void json::push_back(json const& x){
	if(!this->is_list()){
		throw json_exception{"Impossibile eseguire il push_back. L'oggetto JSON non è di tipo lista"};
	}else{
		impl::lista *newNode = new impl::lista;
		newNode->valore = x;
		newNode->next = nullptr;
				
		//lista vuota
		if(pimpl->l_front == nullptr){
			pimpl->l_front = newNode;
			pimpl->l_back = newNode;
		}else{
			pimpl->l_back->next = newNode;
			pimpl->l_back = newNode;
		}
	}
}

//insert
void json::insert(std::pair<std::string, json> const& x){
	if(!this->is_dictionary()){
		throw json_exception{"Impossibile eseguire l'insert. L'oggetto JSON non è di tipo dizionario"};
	}else{	
		impl::dizionario *newNode = new impl::dizionario;
		newNode->diz.first = x.first;
		newNode->diz.second = x.second;
		newNode->next = nullptr;
			
		//lista vuota
		if(pimpl->d_back == nullptr){
			pimpl->d_front = newNode;
			pimpl->d_back = newNode;
		}else{
			pimpl->d_back->next = newNode;
			pimpl->d_back = newNode;
		}
	}
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs){
    if (rhs.is_null()){
        lhs << "null";
    } else if (rhs.is_number()){
        lhs << rhs.get_number();
    } else if (rhs.is_bool()){
        lhs << (rhs.get_bool() ? "true" : "false");
    } else if (rhs.is_string()){
        lhs << '"' << rhs.get_string() << '"';
    } else if (rhs.is_list()){
        lhs << "[";
        bool first = true;
        for(json::const_list_iterator it = rhs.begin_list(); it != rhs.end_list(); it++){
            if(!first){
                lhs << ", ";
            }
            lhs << *it;
            first = false;
        }
        lhs << "]";
    }else if(rhs.is_dictionary()){
        lhs << "{";
        bool first = true;
        for (json::const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary(); it++){
            if (!first){
                lhs << ", ";
            }
            lhs << '"' << it->first << '"' << " : " << it->second;
            first = false;
        }
        lhs << "}";
    }
    return lhs;
}

void parse_number(std::istream& lhs, json& rhs){
    //Parsing di un numero
    double num;
    lhs >> num;
    rhs.set_number(num);
}

void parse_string(std::istream& lhs, json& rhs){
    //Parsing di una stringa
    char c;
    std::string s;
    bool slash = false;
    while(lhs.get(c)){
        if(c == '"' && !slash){
            break;  // Termina il loop quando trova il terminatore di stringa senza escape
        }
        if(c == '\\'){
            slash = !slash;
        }else{
            slash = false;
        }
        s += c;
    }
    rhs.set_string(s); 
}

void parse_null(std::istream& lhs, json& rhs, char c){
    //Parsing di null
    std::string nullStr;
    nullStr += c;
    while(lhs.get(c) && !std::isspace(c) && c != ',' && c != '}' && c != ']'){
        nullStr += c;
    }
    if(nullStr == "null"){
        rhs.set_null();
    }else{
        throw json_exception{"Invalid JSON format. Expected 'null' for null value"};
    }
    lhs.putback(c);    
}

void parse_bool(std::istream& lhs, json& rhs, char c){
    //Parsing di un booleano
    std::string boolStr;
    boolStr += c;
    while(lhs.get(c) && !std::isspace(c) && c != ',' && c != '}' && c != ']'){
        boolStr += c;
    }
    if (boolStr == "true"){
        rhs.set_bool(true);
    }else if (boolStr == "false"){
        rhs.set_bool(false);
    }else{
        throw json_exception{"Invalid JSON format. Expected 'true' or 'false' for boolean value"};
    }
    lhs.putback(c);  
}

void parse_list(std::istream& lhs, json& rhs){
    char c;
    json temp_list; // Variabile temporanea per memorizzare la lista
    temp_list.set_list();

    lhs >> std::ws; // Salta eventuali spazi bianchi iniziali

    while (lhs >> std::skipws >> c && c != ']'){
        lhs.putback(c);

        json value;
        lhs >> value;
        temp_list.push_back(value);

        // Verifica caratteri successivi
        lhs >> std::ws; // Salta eventuali spazi bianchi
        if (!lhs.eof() && lhs.peek() != ']'){
            lhs >> c; // Leggi il prossimo carattere dopo la virgola
            if (c != ',' && c != ']'){
                throw json_exception{"Invalid JSON format. Unexpected character"};
            }
        }
        lhs >> std::ws; // Salta eventuali spazi bianchi
    }

    if (c != ']'){
        throw json_exception{"Invalid JSON format. Expected ']' to close the list"};
    }

    rhs = temp_list; // Assegna la lista temporanea al parametro rhs
}

void parse_dictionary(std::istream& lhs, json& rhs){
	char c;
	json temp_dict; // Variabile temporanea per memorizzare il dizionario
	temp_dict.set_dictionary();

	lhs >> std::ws; // Salta eventuali spazi bianchi iniziali

	while(lhs >> c && c != '}'){
		lhs.putback(c);

		// Parsing della chiave
		std::string key;
		std::getline(lhs, key, ':');

		lhs >> std::ws; // Salta eventuali spazi bianchi

		// Parsing del valore corrente
		json value;
		lhs >> value;

		//il parsing della chiave include due apici in più alla stringa(uno all'inizio e uno alla fine) ed evenruali spazi dopo gli apici e i :
		//quindi li rimuovo
		// Trovo l'indice dell'ultimo carattere non spazio nella stringa
		size_t lastNonSpaceIndex = key.find_last_not_of(' ');

		// Rimuovo gli spazi alla fine della stringa utilizzando la funzione erase
		if (lastNonSpaceIndex != std::string::npos){
			key.erase(lastNonSpaceIndex + 1);
		}

		key = key.substr(1, key.length() - 2);
		// Aggiunge la coppia chiave-valore al dizionario temporaneo
		std::pair<std::string, json> p;
		p.first = key;
		p.second = value;
		temp_dict.insert(p);

		// Verifica caratteri successivi
		lhs >> std::ws; // Salta eventuali spazi bianchi
		if (!lhs.eof() && lhs.peek() != '}'){
			lhs >> c; // Leggi il prossimo carattere dopo la virgola
			if (c != ',' && c != '}'){
				throw json_exception{"Invalid JSON format. Unexpected character"};
				break;
			}
		}
		lhs >> std::ws; // Salta eventuali spazi bianchi
	}

	if (c != '}'){
		throw json_exception{"Invalid JSON format. Expected '}' to close the dictionary"};
	}

	rhs = temp_dict; // Assegna il dizionario temporaneo al parametro rhs
}

std::istream& operator>>(std::istream& lhs, json& rhs){
    char c;
    lhs >> std::skipws >> c;

    if(c == '"'){
        try{
            parse_string(lhs,rhs);
        }catch(const json_exception& ecc){
            std::cerr << ecc.msg << "\n";
        }
    }else if (isdigit(c) || c == '-' || c == '.'){
        lhs.putback(c);  // Rimette il carattere letto nello stream per convertirlo in double
        try{
            parse_number(lhs,rhs);
        }catch(const json_exception& ecc){
            std::cerr << ecc.msg << "\n";
        }
    }else if (c == 'n'){
        try{
            parse_null(lhs,rhs,c);
        }catch(const json_exception& ecc){
            std::cerr << ecc.msg << "\n";
        }
    }else if(c == 't' || c == 'f'){
        try{
            parse_bool(lhs,rhs,c);
        }catch(const json_exception& ecc){
            std::cerr << ecc.msg << "\n";
        }
    }else if(c == '['){
        try{
            rhs.set_list();
            parse_list(lhs,rhs);
        }catch(const json_exception& ecc){
            std::cerr << ecc.msg << "\n";
        }
    }else if(c == '{'){
        try{
            rhs.set_dictionary();
            parse_dictionary(lhs,rhs);
        }catch(const json_exception& ecc){
            std::cerr << ecc.msg << "\n";
        }
    }else{
		throw json_exception{"Invalid JSON format. Unexpected character"};
    }

    return lhs;
}

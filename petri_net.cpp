#include "petri_net.h"

// Places function

Place::Place(string name = "", int nT = 0) {
    this->name = name;
    this->nTokens = nT;
}

void Place::addToken(int nT) {
    this->nTokens += nT;
}

void Place::minusToken(int nT) {
    if (this->nTokens < nT) return;
    else {
        this->nTokens -= nT;
    }
}

int Place::getToken() {
    return this->nTokens;
}

//Transititon
void Transition::addArcOut(Place* p) {
    out.push_back(p);
}

void Transition::addArcIn(Place* p) {
    in.push_back(p);
}

bool Transition::isEnabled() {
    for (auto it = in.begin(); it != in.end(); ++it) {
        if ((*it)->getToken() == 0) return false;
    }

    return true;
}

void Transition::firing() {
    if (this->isEnabled()) {
        for (auto it = in.begin(); it != in.end(); ++it) {
            (*it)->minusToken(1);
        }
        for (auto it = out.begin(); it != out.end(); ++it) {
            (*it)->addToken(1);
        }
    }
}

// Net function
Place* Net::getPlace(string name) {
    for (auto it = p.begin(); it != p.end(); ++it) {
        if ((*it)->name == name) return *it;
    }
    return nullptr;
}

Transition* Net::getTrans(string name) {
    for (auto it = t.begin(); it != t.end(); ++it) {
        if ((*it)->name == name) return *it;
    }

    return nullptr;
}

void Net::addPlace(string name, int token = 0) {
    p.push_back(new Place(name, token));
}

void Net::addTransition(string name) {
    t.push_back(new Transition(name));
}

void Net::addArcOut(string t, string p) {
    Place* pP = getPlace(p);
    Transition* pT = getTrans(t);

    if (pP && pT) pT->addArcOut(pP);
}

void Net::addArcIn(string p, string t) {
    Place* pP = getPlace(p);
    Transition* pT = getTrans(t);

    if (pP && pT) pT->addArcIn(pP);
}

void Net::addArc(string sou, string des) {
    bool isIn = sou[0] == 'p' ? 1 : 0;
    if (isIn) addArcIn(sou, des);
    else {
        addArcOut(sou, des);
    }
}

bool Net::changStage() {
    for (auto it = t.begin(); it != t.end(); ++it) {
        if ((*it)->isEnabled()) {
            (*it)->firing();
            return true;
        }
    }
    return false;
}

string Net::toString() {
    string res = "[";
    for (auto it = p.begin(); it != p.end(); ++it) {
        res += (*it)->name + "(" + to_string((*it)->getToken()) + "), ";
    }
    res = res.length() ? res.substr(0, res.length() - 2) : res;
    res += "]";
    return res;
}
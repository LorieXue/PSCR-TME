#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	int debite = 0;
	if (debiteur.try_lock()) {
		if(crediteur.try_lock()){

			if (debite=debiteur.debiter(val)) {
				std::cout << "transfert de " << deb << " à " << cred << " : " << val << std::endl;
					crediteur.crediter(val);
			} else {
				std::cout << "transfert echouee de " << deb << " à " << cred << " : " << val << std::endl;
			}
			crediteur.unlock();
		} else {
			std::cout << "transfert echouee de " << deb << " à " << cred << " : " << val << std::endl;
		}
		debiteur.unlock();
	} else {
		std::cout << "transfert echouee de " << deb << " à " << cred << " : " << val << std::endl;
	}
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}

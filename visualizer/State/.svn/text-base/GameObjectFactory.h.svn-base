//Ben Murrell
#pragma once

#include "GameObject.h"

#include <string>
#include <map>
using std::map;
using std::string;


class GameObjectFactory
{
	public:
		~GameObjectFactory();
		static GameObjectFactory* instance();
		
		GameObject* create(const SExpr& from);
		void registerHandler(string typeName, GameObject* type);
		bool handles(string typeName);

	private:
		GameObjectFactory();
		map<string, GameObject*> m_templates;

};

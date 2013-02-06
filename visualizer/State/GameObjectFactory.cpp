//Ben Murrell
#include "GameObjectFactory.h"

GameObjectFactory::GameObjectFactory()
{

}

GameObjectFactory::~GameObjectFactory()
{

}

//Singleton
GameObjectFactory* GameObjectFactory::instance()
{
	static GameObjectFactory* inst = new GameObjectFactory();
	return inst;
}


GameObject* GameObjectFactory::create(const SExpr& from)
{
	if(m_templates.count(from[0]) == 0)
	{
		return 0;
	}

	if(m_templates[from[0]] == 0)
	{
		return 0;
	}

	GameObject* ret = m_templates[from[0]]->clone();
	ret->fromSExpr(from);
	return ret;
}


void GameObjectFactory::registerHandler(string typeName, GameObject* type)
{
	m_templates[typeName] = type;
}

bool GameObjectFactory::handles(string typeName)
{
	return (m_templates.count(typeName) > 0);
}
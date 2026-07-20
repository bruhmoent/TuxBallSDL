//  TuxBall
//  Copyright (C) 2022 bruhmoent
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ECS/ECS.h"

ComponentID
getComponentID()
{
  static ComponentID lastID = 0u;
  return lastID++;
}

void
Component::init()
{
}

void
Component::update(float dt)
{
}

void
Component::draw()
{
}

Component::~Component() {}

Entity::Entity(Manager& mManager)
  : manager(mManager)
{
}

void
Entity::update(float dt)
{
  for (auto& c : components)
    c->update(dt);
}

void
Entity::draw()
{
  for (auto& c : components)
    c->draw();
}

bool
Entity::isActive() const
{
  return active;
}

void
Entity::destroy()
{
  active = false;
}

bool
Entity::hasGroup(Group mGroup)
{
  return groupBitSet[mGroup];
}

void
Entity::addGroup(Group mGroup)
{
  groupBitSet[mGroup] = true;
  manager.AddToGroup(this, mGroup);
}

void
Entity::delGroup(Group mGroup)
{
  groupBitSet[mGroup] = false;
}

void
Manager::update(float dt)
{
  for (auto& e : entities)
    e->update(dt);
}

void
Manager::draw()
{
  for (auto& e : entities)
    e->draw();
}

void
Manager::refresh()
{
  for (auto i(0u); i < maxGroups; i++) {
    auto& v(groupedEntities[i]);
    v.erase(std::remove_if(std::begin(v),
                           std::end(v),
                           [i](Entity* mEntity) {
                             return !mEntity->isActive() ||
                                    !mEntity->hasGroup(i);
                           }),
            std::end(v));
  }

  entities.erase(std::remove_if(std::begin(entities),
                                std::end(entities),
                                [](const std::unique_ptr<Entity>& mEntity) {
                                  return !mEntity->isActive();
                                }),
                 std::end(entities));
}

void
Manager::AddToGroup(Entity* mEntity, Group mGroup)
{
  groupedEntities[mGroup].emplace_back(mEntity);
}

std::vector<Entity*>&
Manager::getGroup(Group mGroup)
{
  return groupedEntities[mGroup];
}

Entity&
Manager::addEntity()
{
  Entity* e = new Entity(*this);
  std::unique_ptr<Entity> uPtr{ e };
  entities.emplace_back(std::move(uPtr));
  return *e;
}
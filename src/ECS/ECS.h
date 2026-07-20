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

#ifndef ECS_HPP
#define ECS_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

ComponentID
getComponentID();

template<typename T>
inline ComponentID
getComponentTypeID() noexcept
{
  static ComponentID typeID = getComponentID();
  return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
  Entity* entity;
  virtual void init();
  virtual void update(float dt);
  virtual void draw();
  virtual ~Component();
};

class Entity
{
private:
  bool active = true;
  std::vector<std::unique_ptr<Component>> components;
  Manager& manager;
  ComponentArray componentArray;
  ComponentBitSet componentBitSet;
  GroupBitSet groupBitSet;

public:
  Entity(Manager& mManager);

  void update(float dt);
  void draw();
  bool isActive() const;
  void destroy();

  bool hasGroup(Group mGroup);
  void addGroup(Group mGroup);
  void delGroup(Group mGroup);

  template<typename T>
  bool hasComponent() const
  {
    return componentBitSet[getComponentTypeID<T>()];
  }

  template<typename T, typename... TArgs>
  T& addComponent(TArgs&&... mArgs)
  {
    T* c(new T(std::forward<TArgs>(mArgs)...));
    c->entity = this;
    std::unique_ptr<Component> uPtr{ c };
    components.emplace_back(std::move(uPtr));

    componentArray[getComponentTypeID<T>()] = c;

    componentBitSet[getComponentTypeID<T>()] = true;

    c->init();
    return *c;
  }

  template<typename T>
  T& getComponent() const
  {
    auto ptr(componentArray[getComponentTypeID<T>()]);
    return *static_cast<T*>(ptr);
  }
};

class Manager
{
private:
  std::vector<std::unique_ptr<Entity>> entities;
  std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
  void update(float dt);
  void draw();
  void refresh();
  void AddToGroup(Entity* mEntity, Group mGroup);
  std::vector<Entity*>& getGroup(Group mGroup);
  Entity& addEntity();
};

#endif // ECS.HPP
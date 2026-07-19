#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <mutex>

#define SINGLETON_CPP(CLASS)                                                   \
  std::unique_ptr<CLASS> CLASS::m_instance;                                    \
  std::once_flag CLASS::m_once_flag;                                           \
                                                                               \
  CLASS& CLASS::get()                                                          \
  {                                                                            \
    std::call_once(m_once_flag,                                                \
                   []() { m_instance = std::make_unique<CLASS>(); });          \
    return *m_instance;                                                        \
  }

#define SINGLETON(CLASS)                                                       \
public:                                                                        \
  static CLASS& get();                                                         \
                                                                               \
  CLASS(const CLASS&) = delete;                                                \
  CLASS& operator=(const CLASS&) = delete;                                     \
  CLASS(CLASS&&) = delete;                                                     \
  CLASS& operator=(CLASS&&) = delete;                                          \
                                                                               \
private:                                                                       \
  static std::unique_ptr<CLASS> m_instance;                                    \
  static std::once_flag m_once_flag;

#endif // SINGLETON_HPP
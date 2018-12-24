#ifndef EXCELLENTEAM_ELLA_CPP_SINGLETON_CHANAMI_SAFE_SINGLETON_H
#define EXCELLENTEAM_ELLA_CPP_SINGLETON_CHANAMI_SAFE_SINGLETON_H

#include <pthread.h>
#include <iostream>


template<typename T>
class Singleton
{
public:
    static T* getInstance();

private:
    static T* m_instance;
    static pthread_mutex_t mu_lock;

    Singleton();
    ~Singleton(){ destroy(); };

    static void destroy();
    void freeSynchronized();
    pthread_mutex_t initSynchronized();

    Singleton(Singleton const&);
    Singleton& operator=(Singleton const&);

};

template<typename T>
Singleton <T>::Singleton()
{
    initSynchronized();
    m_instance = static_cast <T*> (this);
}

template<typename T>
pthread_mutex_t Singleton<T>::mu_lock = initSynchronized();

template<typename T>
T* Singleton<T>::m_instance = NULL;

template <typename T>
pthread_mutex_t Singleton<T>::initSynchronized() {
    if ( pthread_mutex_init ( &Singleton<T>::mu_lock, NULL ) != 0 )
    {
        std::cout << "mutex init failed" << std::endl;
        freeSynchronized();
    }
    return mu_lock;
}

template<typename T>
void Singleton <T>::freeSynchronized()
{
    pthread_mutex_destroy(&Singleton <T>::mu_lock);
}


template<typename T>
T* Singleton<T>::getInstance() {

    pthread_mutex_lock(&Singleton <T>::mu_lock);
    //
    if (m_instance == 0)
    {
        try
        {
            m_instance = new T;
        }
        catch (std::bad_alloc)
        {
            pthread_mutex_unlock(&Singleton <T>::mu_lock);
            throw ;
        }
        //
    }

    pthread_mutex_unlock(&Singleton <T>::mu_lock);

    return m_instance;
}

template<typename T>
void Singleton<T>::destroy()
{
    delete Singleton<T>::m_instance;

    Singleton<T>::m_instance = 0;
    freeSynchronized();
}

#endif

#ifndef EXCELLENTEAM_ELLA_CPP_SINGLETON_CHANAMI_SAFE_SINGLETON_H
#define EXCELLENTEAM_ELLA_CPP_SINGLETON_CHANAMI_SAFE_SINGLETON_H

#include <pthread.h>
#include <iostream>


template<typename T>
class Singleton
{
public:
    static T* getInstance();
    static void destroy();

private:
    Singleton(Singleton const&){};
    Singleton& operator=(Singleton const&){};


protected:
    static T* m_instance;

    pthread_mutex_t lock;

    void initSynchronized();
    void freeSynchronized();
    Singleton();
    ~Singleton(){ freeSynchronized(); };
};

template<typename T>
Singleton <T>::Singleton()
{
    m_instance = static_cast <T*> (this);
    initSynchronized();
}

template<typename T>
void Singleton <T>::initSynchronized()
{
    pthread_mutex_init(&Singleton <T>::lock, NULL);
}

template<typename T>
void Singleton <T>::freeSynchronized()
{
    pthread_mutex_destroy(&Singleton <T>::lock);
}

template<typename T>
T* Singleton<T>::m_instance = 0;

template<typename T>
T* Singleton<T>::getInstance() {

    pthread_mutex_lock(&Singleton <T>::lock);
    //
    if (m_instance == NULL)
    {
        pthread_mutex_lock(&Singleton <T>::lock);

        if (m_instance == NULL)
        {
            try
            {
                m_instance = new T;
            }
            catch (std::bad_alloc)
            {
                pthread_mutex_unlock(&Singleton <T>::lock);
                throw ;
            }
            //
        }

        pthread_mutex_unlock(&Singleton <T>::lock);
    }
    return m_instance;
}

template<typename T>
void Singleton<T>::destroy()
{

    delete Singleton<T>::m_instance;

    Singleton<T>::m_instance = 0;
}

#endif

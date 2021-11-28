#pragma once

/*
    TODO:
    Revoir Template.
    Réaliser un singleton thread-safe.
    
    Attention ce singleton n'est pas thread-safe.
*/

template <class T>

class Singleton
{
    protected:
    Singleton() {} // Empêche l'instanciation en dehors de la classe.
    virtual ~Singleton() {}

    public:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);

    static T& getInstance()
    {
        static T instance;
        return instance;
    }
};


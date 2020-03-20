#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

template<class object>
void SafeDelete(object *ptr)
{
    if (ptr != NULL)
    {
        delete ptr;
        ptr = NULL;
    }
}

template<class object>
void SafeDeletes(object *ptr)
{
    if (ptr != NULL)
    {
        delete[] ptr;
        ptr = NULL;
    }
}


#endif // GLOBALFUNCTIONS_H

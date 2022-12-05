
#ifndef PYTHON_EXPORT_H
#define PYTHON_EXPORT_H

#ifdef PYTHON_STATIC_DEFINE
#  define PYTHON_EXPORT
#  define PYTHON_NO_EXPORT
#else
#  ifndef PYTHON_EXPORT
#    ifdef qgispython_EXPORTS
        /* We are building this library */
#      define PYTHON_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PYTHON_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PYTHON_NO_EXPORT
#    define PYTHON_NO_EXPORT 
#  endif
#endif

#ifndef PYTHON_DEPRECATED
#  define PYTHON_DEPRECATED __declspec(deprecated)
#endif

#ifndef PYTHON_DEPRECATED_EXPORT
#  define PYTHON_DEPRECATED_EXPORT PYTHON_EXPORT PYTHON_DEPRECATED
#endif

#ifndef PYTHON_DEPRECATED_NO_EXPORT
#  define PYTHON_DEPRECATED_NO_EXPORT PYTHON_NO_EXPORT PYTHON_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PYTHON_NO_DEPRECATED
#    define PYTHON_NO_DEPRECATED
#  endif
#endif

#endif /* PYTHON_EXPORT_H */

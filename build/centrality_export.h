
#ifndef CENTRALITY_EXPORT_H
#define CENTRALITY_EXPORT_H

#ifdef CENTRALITY_STATIC_DEFINE
#  define CENTRALITY_EXPORT
#  define CENTRALITY_NO_EXPORT
#else
#  ifndef CENTRALITY_EXPORT
#    ifdef Centrality_EXPORTS
        /* We are building this library */
#      define CENTRALITY_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define CENTRALITY_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef CENTRALITY_NO_EXPORT
#    define CENTRALITY_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef CENTRALITY_DEPRECATED
#  define CENTRALITY_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CENTRALITY_DEPRECATED_EXPORT
#  define CENTRALITY_DEPRECATED_EXPORT CENTRALITY_EXPORT CENTRALITY_DEPRECATED
#endif

#ifndef CENTRALITY_DEPRECATED_NO_EXPORT
#  define CENTRALITY_DEPRECATED_NO_EXPORT CENTRALITY_NO_EXPORT CENTRALITY_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CENTRALITY_NO_DEPRECATED
#    define CENTRALITY_NO_DEPRECATED
#  endif
#endif

#endif

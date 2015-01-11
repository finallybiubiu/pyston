// PYSTON_NOEXCEPT This file is originally from CPython 2.7, with modifications for Pyston

#ifndef Py_DICTOBJECT_H
#define Py_DICTOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif


/* Dictionary object type -- mapping from hashable object to object */

/* The distribution includes a separate file, Objects/dictnotes.txt,
   describing explorations into dictionary design and optimization.
   It covers typical dictionary use patterns, the parameters for
   tuning dictionaries, and several ideas for possible optimizations.
*/

/*
There are three kinds of slots in the table:

1. Unused.  me_key == me_value == NULL
   Does not hold an active (key, value) pair now and never did.  Unused can
   transition to Active upon key insertion.  This is the only case in which
   me_key is NULL, and is each slot's initial state.

2. Active.  me_key != NULL and me_key != dummy and me_value != NULL
   Holds an active (key, value) pair.  Active can transition to Dummy upon
   key deletion.  This is the only case in which me_value != NULL.

3. Dummy.  me_key == dummy and me_value == NULL
   Previously held an active (key, value) pair, but that was deleted and an
   active pair has not yet overwritten the slot.  Dummy can transition to
   Active upon key insertion.  Dummy slots cannot be made Unused again
   (cannot have me_key set to NULL), else the probe sequence in case of
   collision would have no way to know they were once active.

Note: .popitem() abuses the me_hash field of an Unused or Dummy slot to
hold a search finger.  The me_hash field of Unused or Dummy slots has no
meaning otherwise.
*/

// Pyston change: comment this out since this is not the format we're using
#if 0
/* PyDict_MINSIZE is the minimum size of a dictionary.  This many slots are
 * allocated directly in the dict object (in the ma_smalltable member).
 * It must be a power of 2, and at least 4.  8 allows dicts with no more
 * than 5 active entries to live in ma_smalltable (and so avoid an
 * additional malloc); instrumentation suggested this suffices for the
 * majority of dicts (consisting mostly of usually-small instance dicts and
 * usually-small dicts created to pass keyword arguments).
 */
#define PyDict_MINSIZE 8

typedef struct {
    /* Cached hash code of me_key.  Note that hash codes are C longs.
     * We have to use Py_ssize_t instead because dict_popitem() abuses
     * me_hash to hold a search finger.
     */
    Py_ssize_t me_hash;
    PyObject *me_key;
    PyObject *me_value;
} PyDictEntry;

/*
To ensure the lookup algorithm terminates, there must be at least one Unused
slot (NULL key) in the table.
The value ma_fill is the number of non-NULL keys (sum of Active and Dummy);
ma_used is the number of non-NULL, non-dummy keys (== the number of non-NULL
values == the number of Active items).
To avoid slowing down lookups on a near-full table, we resize the table when
it's two-thirds full.
*/
typedef struct _dictobject PyDictObject;
struct _dictobject {
    PyObject_HEAD
    Py_ssize_t ma_fill;  /* # Active + # Dummy */
    Py_ssize_t ma_used;  /* # Active */

    /* The table contains ma_mask + 1 slots, and that's a power of 2.
     * We store the mask instead of the size because the mask is more
     * frequently needed.
     */
    Py_ssize_t ma_mask;

    /* ma_table points to ma_smalltable for small tables, else to
     * additional malloc'ed memory.  ma_table is never NULL!  This rule
     * saves repeated runtime null-tests in the workhorse getitem and
     * setitem calls.
     */
    PyDictEntry *ma_table;
    PyDictEntry *(*ma_lookup)(PyDictObject *mp, PyObject *key, long hash);
    PyDictEntry ma_smalltable[PyDict_MINSIZE];
};
#endif
PyAPI_DATA(PyTypeObject*) list_cls;
#define PyList_Type (*list_cls)

// Pyston change: these are no longer static objects:
#if 0
PyAPI_DATA(PyTypeObject) PyDict_Type;
PyAPI_DATA(PyTypeObject) PyDictIterKey_Type;
PyAPI_DATA(PyTypeObject) PyDictIterValue_Type;
PyAPI_DATA(PyTypeObject) PyDictIterItem_Type;
PyAPI_DATA(PyTypeObject) PyDictKeys_Type;
PyAPI_DATA(PyTypeObject) PyDictItems_Type;
PyAPI_DATA(PyTypeObject) PyDictValues_Type;
#endif
PyAPI_DATA(PyTypeObject*) dict_cls;
#define PyDict_Type (*dict_cls)
PyAPI_DATA(PyTypeObject*) dictiterkey_cls;
#define PyDictIterKey_Type (*dictiterkey_cls)
PyAPI_DATA(PyTypeObject*) dictitervalue_cls;
#define PyDictIterValue_Type (*dictitervalue_cls)
PyAPI_DATA(PyTypeObject*) dictiteritem_cls;
#define PyDictIterItem_Type (*dictiteritem_cls)
PyAPI_DATA(PyTypeObject*) dictkeys_cls;
#define PyDictKeys_Type (*dictkeys_cls)
PyAPI_DATA(PyTypeObject*) dictitems_cls;
#define PyDictItems_Type (*dictitems_cls)
PyAPI_DATA(PyTypeObject*) dictvalues_cls;
#define PyDictValues_Type (*dictvalues_cls)

// Pyston changes: these aren't direct macros any more [they potentially could be though]
PyAPI_FUNC(bool) PyDict_Check(PyObject*) PYSTON_NOEXCEPT;
#if 0
#define PyDict_Check(op) \
                 PyType_FastSubclass(Py_TYPE(op), Py_TPFLAGS_DICT_SUBCLASS)
#endif
#define PyDict_CheckExact(op) (Py_TYPE(op) == &PyDict_Type)
#define PyDictKeys_Check(op) (Py_TYPE(op) == &PyDictKeys_Type)
#define PyDictItems_Check(op) (Py_TYPE(op) == &PyDictItems_Type)
#define PyDictValues_Check(op) (Py_TYPE(op) == &PyDictValues_Type)
/* This excludes Values, since they are not sets. */
# define PyDictViewSet_Check(op) \
    (PyDictKeys_Check(op) || PyDictItems_Check(op))

PyAPI_FUNC(PyObject *) PyDict_New(void) PYSTON_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyDict_GetItem(PyObject *mp, PyObject *key) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) PyDict_SetItem(PyObject *mp, PyObject *key, PyObject *item) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) PyDict_DelItem(PyObject *mp, PyObject *key) PYSTON_NOEXCEPT;
PyAPI_FUNC(void) PyDict_Clear(PyObject *mp) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) PyDict_Next(
    PyObject *mp, Py_ssize_t *pos, PyObject **key, PyObject **value) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) _PyDict_Next(
    PyObject *mp, Py_ssize_t *pos, PyObject **key, PyObject **value, long *hash) PYSTON_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyDict_Keys(PyObject *mp) PYSTON_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyDict_Values(PyObject *mp) PYSTON_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyDict_Items(PyObject *mp) PYSTON_NOEXCEPT;
PyAPI_FUNC(Py_ssize_t) PyDict_Size(PyObject *mp) PYSTON_NOEXCEPT;
PyAPI_FUNC(PyObject *) PyDict_Copy(PyObject *mp) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) PyDict_Contains(PyObject *mp, PyObject *key) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) _PyDict_Contains(PyObject *mp, PyObject *key, long hash) PYSTON_NOEXCEPT;
PyAPI_FUNC(PyObject *) _PyDict_NewPresized(Py_ssize_t minused) PYSTON_NOEXCEPT;
PyAPI_FUNC(void) _PyDict_MaybeUntrack(PyObject *mp) PYSTON_NOEXCEPT;

/* PyDict_Update(mp, other) is equivalent to PyDict_Merge(mp, other, 1). */
PyAPI_FUNC(int) PyDict_Update(PyObject *mp, PyObject *other) PYSTON_NOEXCEPT;

/* PyDict_Merge updates/merges from a mapping object (an object that
   supports PyMapping_Keys() and PyObject_GetItem()).  If override is true,
   the last occurrence of a key wins, else the first.  The Python
   dict.update(other) is equivalent to PyDict_Merge(dict, other, 1).
*/
PyAPI_FUNC(int) PyDict_Merge(PyObject *mp,
                                   PyObject *other,
                                   int override) PYSTON_NOEXCEPT;

/* PyDict_MergeFromSeq2 updates/merges from an iterable object producing
   iterable objects of length 2.  If override is true, the last occurrence
   of a key wins, else the first.  The Python dict constructor dict(seq2)
   is equivalent to dict={}; PyDict_MergeFromSeq(dict, seq2, 1).
*/
PyAPI_FUNC(int) PyDict_MergeFromSeq2(PyObject *d,
                                           PyObject *seq2,
                                           int override) PYSTON_NOEXCEPT;

PyAPI_FUNC(PyObject *) PyDict_GetItemString(PyObject *dp, const char *key) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) PyDict_SetItemString(PyObject *dp, const char *key, PyObject *item) PYSTON_NOEXCEPT;
PyAPI_FUNC(int) PyDict_DelItemString(PyObject *dp, const char *key) PYSTON_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif /* !Py_DICTOBJECT_H */

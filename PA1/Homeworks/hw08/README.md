# Sorted linked list II

The task is to implement three functions that ease handling and sorting of linked lists. This problem is an extension of the basic linked list sorting problem where the sorting may be parameterized with a custom comparator. We recommend to first develop the program for the basic task and then update the working program.

The functions need to fit into the following declarations:

TITEM

This structure represents an item of a singly-linked list. The declaration is fixed in the testing environment, your implementation must use the existing declaration (it cannot modify it in any way). The structure contains the following fields:
- m_Next a pointer to the next element in the linked list or NULL for the last element in the list,
- m_Name a string with the name of the element,
- m_Secret some secret data associated with the item. With the exception of function newItem, your implementation does not need to access/modify the data.

TITEM * newItem ( const char * name, TITEM * next )

This function is a helper function that eases the creation of a linked list. The function dynamically allocates the new item, it initializes the fields, and finally, it returns the pointer to that newly created item. Fields m_Next and m_Name must be initialized from the corresponding parameters, field m_Secret is to be filled with zero bytes ('\0'). The implementation is your task.

void freeList ( TITEM * l )

The function deallocates the memory used by the linked list. The parameter is a pointer to the first element in the list. The implementation is your task.

TITEM * sortListCmp ( TITEM * l, int ascending, int (*cmp)(const TITEM *, const TITEM *) )

The function is used to sort the elements in the given linked list. The parameter is a pointer to the first element of the list to sort (l), the requested sort order (ascending), and a comparator function cmp. The function re-arranges the elements in the list such that the elements follow the requested sort order. The function <b>must not</b> free the elements in the original list (and return their copies). Instead, the function <b>must</b> use the existing elements, it just needs to modify the links. Return value is the pointer to the first element of the newly re-arranged list.

<p>The sorting uses the provided comparator function cmp. Whenever the sorting algorithm needs to compare a pair of elements, it is expected to call the provided function. The comparator is identical to the comparator in the standard qsort function: it returns negative / zero / positive result if the first parameter is smaller than / equal to / greater than the second parameter. Furthermore, there is the sort order parameter ascending. The sort order is either ascending (parameter ascending is not zero), or the sort order is descending (parameter ascending is zero). The function must perform stable sorting.</p>

<p>Submit a source file with the implementation of the required functions. Further, the source file must include your auxiliary functions which are called from sortList / newItem / freeList. The function will be called from the testing environment, thus, it is important to adhere to the required interface. Use the attached source code as a basis for your development. There is an example main with some tests in the attached source. These values will be used in the basic test. Please note the header files as well as main is nested in a conditional compile block (#ifdef/#endif). Please keep these conditional compile block in place. They are present to simplify the development. When compiling on your computer, the headers and main will be present as usual. On the other hand, the headers and main will "disappear" when compiled by Progtest. Thus, your testing main will not interfere with the testing environment's main.</p>

<b>Advice:</b>
- Copy the sample code from the archive and use it as a base for your development.
- The main in your program may be modified (e.g. a new test may be included). The conditional compile block must remain, however.
- The testing environment disables some functions. In particular, library functions qsort and qsort_r are not available (they are not designated for linked lists anyway).
- The input linked lists are quite short in the mandatory tests. Sorting efficiency is not very important.
- The first bonus test checks the time efficiency of the sorting algorithm. It inputs very long linked lists, thus quadratic algorithms exceed the time limit.
- The first bonus test checks the time and memory efficiency of the sorting algorithm. Similarly to the first bonus, it inputs very long linked lists. Moreover, the available memory is limited, only a few hundred KiB is available.
- The attached source lists the header files available in the testing environment. Other header files are not present, moreover, you cannot include them by means of #include directives in your submitted code.
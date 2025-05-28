from pydatastructs.dllist import DoublyLinkedList

def test_append_and_to_list():
    lst = DoublyLinkedList()
    lst.append(1)
    lst.append(2)
    assert lst.to_list() == [1, 2]

def test_remove_end():
    lst = DoublyLinkedList()
    lst.append(1)
    lst.append(2)
    assert lst.remove(1) == 1
    assert lst.to_list() == [2]

def test_remove_start():
    lst = DoublyLinkedList()
    lst.append(1)
    lst.append(2)
    assert lst.remove(2) == 2
    assert lst.to_list() == [1]

def test_remove_unique():
    lst = DoublyLinkedList()
    lst.append(1)
    assert lst.remove(1) == 1
    assert lst.to_list() == []

def test_remove_and_append():
    lst = DoublyLinkedList()
    lst.append(1)
    assert lst.remove(1) == 1
    lst.append(1)
    assert lst.to_list() == [1]

def test_remove_empty():
    lst = DoublyLinkedList()
    assert lst.remove(1) is None
    lst.append(1)
    assert lst.to_list() == [1]

def test_remove_invalid():
    lst = DoublyLinkedList()
    lst.append(1)
    assert lst.remove(2) is None
    assert lst.to_list() == [1]

def test_prepend():
    lst = DoublyLinkedList()
    lst.append(1)
    lst.prepend(2)
    assert lst.to_list() == [2, 1]

def test_prepend_empty():
    lst = DoublyLinkedList()
    lst.prepend(2)
    assert lst.to_list() == [2]

def test_prepend_multiple():
    lst = DoublyLinkedList()
    lst.prepend(2)
    lst.prepend(3)
    lst.prepend(4)
    assert lst.to_list() == [4,3,2]

def test_len_unique():
    lst = DoublyLinkedList()
    lst.prepend(2)
    assert len(lst) == 1

def test_len_multiple():
    lst = DoublyLinkedList()
    lst.prepend(2)
    lst.prepend(2)
    lst.prepend(2)
    assert len(lst) == 3

def test_len_empty():
    lst = DoublyLinkedList()
    assert len(lst) == 0
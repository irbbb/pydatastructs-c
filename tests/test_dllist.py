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

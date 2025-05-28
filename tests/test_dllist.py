from pydatastructs.dllist import DoublyLinkedList

def test_append_and_to_list():
    lst = DoublyLinkedList()
    lst.append(1)
    lst.append(2)
    assert lst.to_list() == [1, 2]

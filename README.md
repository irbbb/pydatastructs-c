# pydatastructs-c
Collection of classic data structures implemented in C and exposed to Python as high-performance modules.

**pydatastructs-c** is a lightweight library of classic data structures implemented in C and exposed as Python modules via the Python C API. The project is designed to be fast, educational, and easy to extend — perfect for performance-focused Python developers and systems programming learners.

---

## 📌 Why This Project?

While Python is great for rapid development, many core data structures can benefit from native C performance. This project bridges the gap, providing Python users with efficient, low-level implementations without sacrificing ease of use.

This is also a great portfolio project for learning and demonstrating:

- How data structures work internally
- C ↔ Python integration
- Memory management and performance considerations
- Clean API design

---

## 📦 Current Data Structures

| Structure           | Description                                         | Status |
|---------------------|-----------------------------------------------------|--------|
| `DoublyLinkedList`  | Classic double-ended linked list (prepend/append)   | ✅ Done |
| `MinHeap`           | Binary heap for priority queue operations           | ✅ Done |
| `HashTable`         | Hash map with collision resolution                  | 🔜 Planned |
| `Trie`              | Prefix tree for fast string queries                 | 🔜 Planned |
| `UnionFind`         | Disjoint-set structure for component tracking       | 🔜 Planned |

More structures (queues, stacks, BSTs, etc.) will be added progressively.

---

## 🧪 Example Usage in Python

```python
from pydatastructs.dllist import DoublyLinkedList

lst = DoublyLinkedList()
lst.append(10)
lst.prepend(5)
print(lst.to_list())  # Output: [5, 10]
lst.remove(10)
print(len(lst))       # Output: 1
```

---


## Running Tests
The project includes simple Python unit tests for each data structure.
To run the tests:

```bash
pip install pytest
pytest tests/
```

Make sure the module is installed:

```bash
pip install .
```

---

## ⚙️ Installation
You can build and install the package locally with:
```bash
pip install .
```

Make sure you have a C compiler installed (e.g., gcc via MinGW on Windows or gcc/clang on Linux/macOS).

---

## 🧱 Project Structure

```r
pydatastructs-c/
├── src/              # C implementations (dllist.c, min_heap.c, etc.)
├── bindings/         # Python C API wrappers
├── tests/            # Python unit tests
├── setup.py          # Build script
└── README.md
```

---

## 🚀 Goals
- Create a modern, minimalistic C data structures library
- Provide safe and clean Python bindings
- Use this as a learning and showcase project

---

## 📄 License
MIT License. Free to use and modify.

------

## 💡 Contributing
This project is still early stage and open to collaboration. Feel free to fork, suggest new structures, or help improve performance and testing.

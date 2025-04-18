# ✒️ Gibberish Generator

A simple, fast, and elegant Markov Chain–based text generator written in C.

This program reads a plain text file, builds a probabilistic model using prefix–suffix chains, and generates semi-random text that mimics the style and vocabulary of the original source.

---

## 📦 Features

- 📚 **Text Training** via Markov Chains (`NPREF = 2`)
- ✨ **Random Text Generation** based on prefix–suffix probability
- 🧠 **Lightweight & Portable**
  - No external dependencies
  - ANSI C compatible
- 🧪 Deterministic structure with randomized creativity

---

## 🛠️ Build Instructions

To compile the program, run:

```sh
git clone https://github.com/321BadgerCode/gibberish_generator.git
cd ./gibberish_generator/
gcc ./main.c -o ./gibberish_generator
```

Requires a standard C compiler (GCC, Clang, etc.).

---

## 🚀 Usage

```sh
./gibberish_generator <input.txt> [nwords]
```

- `<input.txt>` – a plain text file to train the Markov model
- `[nwords]` – optional number of words to generate (default: 100)

---

## 🔍 Example

Input file (`example.txt`):

```
the quick brown fox jumps over the lazy dog
the quick brown bear sleeps
the lazy fox plays chess
the energetic dog eats a bone
the stationary bone gets eaten
```

Command:

```sh
./gibberish_generator ./example.txt 50
```

Sample Output:

```text
the quick brown bear sleeps the lazy dog the quick brown fox jumps over the lazy fox plays chess the energetic dog eats a bone the stationary bone gets eaten
```

Output will vary with each run due to entropy in the Markov model.

---

## 📁 Files

- `main.c` – main program
- `README.md` – this file
- `example.txt` – example input file

---

## 🧹 TODO / Possible Improvements

- [ ] Dynamic prefix length (via CLI)
- [ ] Export model to file
- [ ] Improved sentence parsing using NLP
- [ ] Better clause segmentation
- [ ] Puncuation post-processing

---

## 📜 License

[MIT License](LICENSE)
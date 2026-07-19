# T-Lab: PageRank на графе, не помещающемся в RAM

Реализация PageRank для ориентированных графов, которые не помещаются в оперативную память единственного вычислительного узла. Рёбра хранятся на диске в бинарном формате и читаются блоками, в RAM живут только массивы O(V).

Подробности алгоритма, обоснование выбора, расчёт памяти - в [REPORT.md](REPORT.md).

## Требования

- **Docker** - для сборки и запуска в изолированном окружении.
- **Python 3 с `networkx`** (опционально) - только для пересчёта эталона через `scripts/test.py`. Готовые эталоны для тестов 1-5 лежат в `data/tests_expected/`.

## Сборка и запуск через Docker

```bash
docker build -t tlab .
docker run --rm -v $(pwd)/data:/app/data tlab \
    ./main data/tests/data_1_default.csv data/tests_real_result/data_1_docker.csv
```

**Формат входа:** CSV с заголовком `from,to`, дальше парыы `int32,int32` - по одному ребру на строку.

**Формат выхода:** CSV с заголовком `vertex,rank`, отсортировано по возрастанию исходного id вершины.

## Датасеты

Малые тесты (`data_1` – `data_5`) лежат в репозитории. Большие датасеты нужно скачать отдельно из [SNAP web-Google](https://snap.stanford.edu/data/web-Google.html) и [SNAP LiveJournal](https://snap.stanford.edu/data/soc-LiveJournal1.html). Далее предложены команды, чтобы удобно скачать файлы.

**web-Google** (~876K вершин, 5M рёбер):

```bash
wget https://snap.stanford.edu/data/web-Google.txt.gz -P data/
gunzip data/web-Google.txt.gz
python3 scripts/snap_to_csv.py data/web-Google.txt data/tests/data_6_web-Google.csv
```

**LiveJournal** (~4.8M вершин, 69M рёбер):

```bash
wget https://snap.stanford.edu/data/soc-LiveJournal1.txt.gz -P data/
gunzip data/soc-LiveJournal1.txt.gz
python3 scripts/snap_to_csv.py data/soc-LiveJournal1.txt data/tests/data_7_livejournal.csv
```
## Запуск на скачанных датасетах

После того как датасеты сконвертированы в CSV (см. раздел «Датасеты»), запусти так же через Docker:

```bash
docker run --rm -v $(pwd)/data:/app/data tlab ./main data/tests/data_6_web-Google.csv data/tests_real_result/data_6_result.csv
docker run --rm -v $(pwd)/data:/app/data tlab ./main data/tests/data_7_livejournal.csv data/tests_real_result/data_7_result.csv
```

Аналогично запускается на любом CSV-файле: ./main <input.csv> <output.csv>.

На LiveJournal (~69M рёбер) полный прогон занимает ~2.5 минуты на 16 ядрах (Redmi Book Pro 14-2024)

## Проверка корректности

Пересчитать эталон через networkx:

```bash
python3 scripts/test.py <input.csv> <output_expected.csv>
```

Расхождение результата моего кода с эталоном на уровне 1e-8 - норма (машинная точность double + порог сходимости 1e-10).

## Структура проекта

```
src/main.cpp              - реализация PageRank
scripts/test.py           - эталон на networkx для проверки
scripts/snap_to_csv.py    - конвертер SNAP-формата в CSV
data/tests/               - входные графы
data/tests_expected/      - эталонные результаты
data/tests_real_result/   - результаты моего кода
Dockerfile                - сборка в Ubuntu 22.04
REPORT.md                 - отчёт по решению
README.md                 - инструкия по запуску программы
```
print('Solution for A3, Task 2');
print('Student number: 9890385');
print('Name: Joel Beh Zhirong');
print('---------------------------------------------------------------------');
print('');

// Q1
print('(1) Display all subject codes and titles from the collection.');
print('');
print(
  'db.Subject.find(\n' +
  '  {},\n' +
  '  {_id: 0, "subject.subCode": 1, "subject.subTitle": 1}\n' +
  ').toArray();'
);
print('');

printjson(
  db.Subject.find(
    {},
    {_id: 0, "subject.subCode": 1, "subject.subTitle": 1}
  ).toArray()
);
print('');

// Q2
print('(2) Find all Core subjects with more than 3 assessments.');
print('');
print(
  'db.Subject.aggregate([\n' +
  '  {$match: {"subject.type": "Core"}},\n' +
  '  {$addFields: {numAssessments: {$size: "$subject.assessments"}}},\n' +
  '  {$match: {numAssessments: {$gt: 3}}},\n' +
  '  {$project: {_id: 0, "subject.subCode": 1, "subject.subTitle": 1, numAssessments: 1}}\n' +
  ']).toArray();'
);
print('');

printjson(
  db.Subject.aggregate([
    {$match: {"subject.type": "Core"}},
    {$addFields: {numAssessments: {$size: "$subject.assessments"}}},
    {$match: {numAssessments: {$gt: 3}}},
    {$project: {_id: 0, "subject.subCode": 1, "subject.subTitle": 1, numAssessments: 1}}
  ]).toArray()
);
print('');

// Q3
print('(3) List the subject title, type, and total credit points for subject worth at least 3 credits.');
print('');
print(
  'db.Subject.find(\n' +
  '  {"subject.credit": {$gte: 3}},\n' +
  '  {_id: 0, "subject.subTitle": 1, "subject.type": 1, "subject.credit": 1}\n' +
  ').toArray();'
);
print('');

printjson(
  db.Subject.find(
    {"subject.credit": {$gte: 3}},
    {_id: 0, "subject.subTitle": 1, "subject.type": 1, "subject.credit": 1}
  ).toArray()
);
print('');

// Q4
print('(4) Display the subject code and total number of assessments for each subject.');
print('');
print(
  'db.Subject.aggregate([\n' +
  '  {$project: {_id: 0, "subject.subCode": 1, numAssessments: {$size: "$subject.assessments"}}}\n' +
  ']).toArray();'
);
print('');

printjson(
  db.Subject.aggregate([
    {$project: {_id: 0, "subject.subCode": 1, numAssessments: {$size: "$subject.assessments"}}}
  ]).toArray()
);
print('');

// Q5
print('(5) List the titles of all subjects whose assessment weights adds up to more than 80%.');
print('');
print(
  'db.Subject.aggregate([\n' +
  '  {$project: {"subject.subTitle": 1, totalWeight: {$sum: "$subject.assessments.weight"}}},\n' +
  '  {$match: {totalWeight: {$gt: 80}}},\n' +
  '  {$project: {_id: 0, "subject.subTitle": 1, totalWeight: 1}}\n' +
  ']).toArray();'
);
print('');

printjson(
  db.Subject.aggregate([
    {$project: {"subject.subTitle": 1, totalWeight: {$sum: "$subject.assessments.weight"}}},
    {$match: {totalWeight: {$gt: 80}}},
    {$project: {_id: 0, "subject.subTitle": 1, totalWeight: 1}}
  ]).toArray()
);
print('');

// Q6
print('(6) Find the subjects that include an Examination as part of their assessment. Show subject code and title only.');
print('');
print(
  'db.Subject.find(\n' +
  '  {"subject.assessments.assessType": "Examination"},\n' +
  '  {_id: 0, "subject.subCode": 1, "subject.subTitle": 1}\n' +
  ').toArray();'
);
print('');

printjson(
  db.Subject.find(
    {"subject.assessments.assessType": "Examination"},
    {_id: 0, "subject.subCode": 1, "subject.subTitle": 1}
  ).toArray()
);
print('');

// Q7
print('(7) List each subject together with its number of Assignments (assessType = "Assignment").');
print('');
print(
  'db.Subject.aggregate([\n' +
  '  {$project: {\n' +
  '    "subject.subCode": 1,\n' +
  '    "subject.subTitle": 1,\n' +
  '    assignmentList: {$filter: {input: "$subject.assessments", as: "a", cond: {$eq: ["$$a.assessType", "Assignment"]}}}\n' +
  '  }},\n' +
  '  {$project: {_id: 0, "subject.subCode": 1, "subject.subTitle": 1, numAssignments: {$size: "$assignmentList"}}}\n' +
  ']).toArray();'
);
print('');

printjson(
  db.Subject.aggregate([
    {
      $project: {
        "subject.subCode": 1,
        "subject.subTitle": 1,
        assignmentList: {
          $filter: {
            input: "$subject.assessments",
            as: "a",
            cond: {$eq: ["$$a.assessType", "Assignment"]}
          }
        }
      }
    },
    {
      $project: {
        _id: 0,
        "subject.subCode": 1,
        "subject.subTitle": 1,
        numAssignments: {$size: "$assignmentList"}
      }
    }
  ]).toArray()
);
print('');

// Q8
print('(8) Find all subjects that have Pearson as one of their book publishers.');
print('');
print(
  'db.Subject.find(\n' +
  '  {"subject.book.publisher": "Pearson"},\n' +
  '  {_id: 0, "subject.subCode": 1, "subject.subTitle": 1}\n' +
  ').toArray();'
);
print('');

printjson(
  db.Subject.find(
    {"subject.book.publisher": "Pearson"},
    {_id: 0, "subject.subCode": 1, "subject.subTitle": 1}
  ).toArray()
);
print('');

// Q9
print('(9) List the bookTitle, author(s), and publisher for all reference books (bookType = "reference").');
print('');
print(
  'db.Subject.aggregate([\n' +
  '  {$unwind: "$subject.book"},\n' +
  '  {$match: {"subject.book.bookType": "reference"}},\n' +
  '  {$project: {_id: 0, bookTitle: "$subject.book.bookTitle", author: "$subject.book.author", publisher: "$subject.book.publisher"}}\n' +
  ']).toArray();'
);
print('');

printjson(
  db.Subject.aggregate([
    {$unwind: "$subject.book"},
    {$match: {"subject.book.bookType": "reference"}},
    {$project: {
      _id: 0,
      bookTitle: "$subject.book.bookTitle",
      author: "$subject.book.author",
      publisher: "$subject.book.publisher"
    }}
  ]).toArray()
);
print('');

// Q10
print('(10) Find subjects where at least one textbook was published before 2010. Display the subject code and textbook title.');
print('');
print(
  'db.Subject.aggregate([\n' +
  '  {$unwind: "$subject.book"},\n' +
  '  {$match: {"subject.book.bookType": "textbook", "subject.book.yearPub": {$lt: 2010}}},\n' +
  '  {$project: {_id: 0, "subject.subCode": 1, bookTitle: "$subject.book.bookTitle"}}\n' +
  ']).toArray();'
);
print('');

printjson(
  db.Subject.aggregate([
    {$unwind: "$subject.book"},
    {$match: {"subject.book.bookType": "textbook", "subject.book.yearPub": {$lt: 2010}}},
    {$project: {
      _id: 0,
      "subject.subCode": 1,
      bookTitle: "$subject.book.bookTitle"
    }}
  ]).toArray()
);
print('');

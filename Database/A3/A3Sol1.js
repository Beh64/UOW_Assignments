print('Solution for A3, Task 1');
print('Student number: 9890385');
print('Name: Joel Beh Zhirong');
print('---------------------------------------------------------------------');

print('(0) Select database bankDB and drop collection accounts (if it exists.)');
print('');
print('db = db.getSiblingDB("bankDB")');
print('db.accounts.drop();');
print('');

db = db.getSiblingDB("bankDB");
db.accounts.drop();

//Account 1
var acc1 = {
    "ACCOUNT":{
        "acctNumber":"A1001",
        "balance":1201.00,
        //CUSTOMER owns ACCOUNT
        "owns":{
            "CUSTOMER":{
                "custId":"C001",
                "firstName":"Alice",
                "lastName":"Tan"
            }
        },

        //account is_opened_at BRANCH
        "is_opened_at":{
            "BRANCH":{
                "brchName":"Punggol",
                "address": "1 Punggol Road, Singapore"
            }
        },

        //process is attended_by EMPLOYEE, who works_in a BRANCH
        "attended_by":{
            "EMPLOYEE":{
                "eNum":"E001",
                "fullName":"Elon Lim",
                "spouseName":["Britney Lim"],
                "works_in":{
                    "BRANCH":{
                        "brchName":"Punggol",
                        "address":"1 Punggol Road, Singapore"
                    }
                }
            }
        }
    }
};

//Account 2
var acc2 = {
    "ACCOUNT":{
        "acctNumber":"A2002",
        "balance":5493.50,
        "owns":{
            "CUSTOMER":{
                "custId":"C002",
                "firstName":"Bruce",
                "lastName":"Lee"
            }
        },

        "is_opened_at":{
            "BRANCH":{
            "brchName":"Airport",
            "address":"60 Airport Boulevard, Singapore"
            }
        },
        "attended_by":{
            "EMPLOYEE":{
                "eNum":"E002",
                "fullName":"Amos Yee",
                "spouseName":[],
                "works_in":{
                    "BRANCH":{
                        "brchName":"Airport",
                        "address":"60 Airport Boulevard, Singapore"
                    }
                }
            }
        }
    }
};

//Account 3
var acc3 = {
    "ACCOUNT":{
        "acctNumber":"A3003",
        "balance":924.70,
        "owns":{
            "CUSTOMER":{
                "custId":"C003",
                "firstName":"Mia",
                "lastName":"Malkovich"
            }
        },
        "is_opened_at":{
            "BRANCH":{
                "brchName":"Tampines",
                "address":"55 Tampines Avenue, Singapore"
            }
        },
        "attended_by":{
            "EMPLOYEE":{
                "eNum":"E003",
                "fullName":"Charlie Kirk",
                "spouseName":["Amanda Kirk", "Taylor Kirk"],
                "works_in":{
                    "BRANCH":{
                        "brchName":"Tampines",
                        "address":"55 Tampines Avenue, Singapore"
                    }
                }
            }
        }
    }
};

function printInsert(label, doc) {
    print(label);
    print('');
    print('db.accounts.insertOne(');
    printjson(doc);      // pretty-prints the BSON document
    print(');');
    print('');
}

// Insert first account document
printInsert('(1) Insert first ACCOUNT document (acctNumber A1001).', acc1);
var res1 = db.accounts.insertOne(acc1);
print('Confirmation message for insertion of A1001');
printjson(res1);
print('');

// Insert second account document
printInsert('(2) Insert second ACCOUNT document (acctNumber A2002).', acc2);
var res2 = db.accounts.insertOne(acc2);
print('Confirmation message for insertion of A2002');
printjson(res2);
print('');

// Insert third account document
printInsert('(3) Insert third ACCOUNT document (acctNumber A3003).', acc3);
var res3 = db.accounts.insertOne(acc3);
print('Confirmation message for insertion of A3003:');
printjson(res3);
print('');

// Display all documents

print('(4) Display all ACCOUNT documents from collection accounts.');
print('');
print('db.accounts.find().pretty();');
print('');

db.accounts.find().pretty();
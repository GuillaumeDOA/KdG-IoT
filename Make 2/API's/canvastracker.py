import requests

token = "12400~M8P1vXZT5hkCNG6LsGY0blnvOUngy4oNDnJFlLwniv0ady6YnAv8ZDsRlyLD4eNx"
base = "https://canvas.kdg.be"

def checkError(response):
    if response.status_code != 200:
        print(f"ERROR: API request '{response.url}' failed")
        exit(1)

def getCourses():
    result = requests.get(f"{base}/api/v1/courses?access_token={token}&enrollment_state=active")
    checkError(result)
    result = result.json()
    res = {}
    for i in result:
        if any(substring in i["name"] for substring in ["Make", "Build", "Improve"]):
            res[i["name"]] = i["id"]
    return res


def getUserID():
    result = requests.get(f"{base}/api/v1/users/self?access_token={token}")
    checkError(result)
    return result.json()["id"]

def getAssignments(courseID):
    result = requests.get(f"{base}/api/v1/courses/{courseID}/assignments?access_token={token}&bucket=past")
    checkError(result)
    return result.json()

def getSubmissions(courseID, assignmentID):
    result = requests.get(f"{base}/api/v1/courses/{courseID}/assignments/{assignmentID}/submissions/{userID}?access_token={token}&include=full_rubric_assessment")
    checkError(result)
    return result.json()

userID = getUserID()
courses = getCourses()

for courseName in courses.keys():
    print(f"{courseName.upper()} ({courses[courseName]})")
    print("----------------------------------------------")
    assignments = getAssignments(courses[courseName])
    for assignment in assignments:
        if assignment['graded_submissions_exist']:
            submission = getSubmissions(courses[courseName], assignment['id'])
            grades = []
            if "full_rubric_assessment" not in submission.keys():
                continue
            for grade in submission['full_rubric_assessment']["data"]:
                grades.append(grade['description'])
            if any(substring in grades for substring in ["Bijna goed","Nog niet goed", "Niet ingediend"]):
                print(f"{assignment['name']} ({assignment["id"]}) --> NIET OK!")
            # else:
            #     print(f"{assignment['name']} ({assignment["id"]}) --> OK!")
    print()

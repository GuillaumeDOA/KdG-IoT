import requests


token = "12400~M8P1vXZT5hkCNG6LsGY0blnvOUngy4oNDnJFlLwniv0ady6YnAv8ZDsRlyLD4eNx"
base = "https://canvas.kdg.be"

def getCourses():
    result = requests.get(f"{base}/api/v1/courses?access_token={token}&enrollment_state=active")
    return result.json()

def getUserID():
    result = requests.get(f"{base}/api/v1/users/self?access_token={token}")
    return result.json()['id']

def getAssignments(courseID):
    result = requests.get(f"{base}/api/v1/courses/{courseID}/assignments?access_token={token}")
    return result.json()

def getSubmissions(courseID, assignmentID):
    result = requests.get(f"{base}/api/v1/courses/{courseID}/assignments/{assignmentID}/submissions/{userID}?access_token={token}&include=full_rubric_assessment")
    return result.json()

userID = getUserID()
courses = getCourses()

for course in courses:
    if any(substring in course['name'] for substring in ['Make', 'Build', 'Improve']):
        print(f"COURSE:  {course['name'].upper()} ({course['id']})")
        print("----------------------------------------------")
        assignments = getAssignments(course['id'])
        for assignment in assignments:
            submission = getSubmissions(course['id'], assignment['id'])
            grades = []
            # for grade in submission['full_rubric_assessment']['data']:
            #     grades.append(grade['description'])
            # if 'Niet Goed' in grades or 'Bijna Goed' in grades:
            print(f"{assignment['name']}: {submission['full_rubric_assessment']}")
        print()

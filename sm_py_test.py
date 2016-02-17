import sm_py

if __name__ == '__main__':
    aliceKeys = sm_py.key_init()["keys"]
    bobKeys = sm_py.key_init()["keys"]
    secret = "test"
    step1 = sm_py.step_1(secret, aliceKeys)
    aliceKeys = step1["keys"]
    print(step1["message"])
    step2 = sm_py.step_2(step1["message"], bobKeys)
    bobKeys = step2["keys"]
    step3 = sm_py.step_3("test", bobKeys)
    bobKeys = step3["keys"]
    print(step3["message"])
    step4 = sm_py.step_4(step3["message"], aliceKeys)
    aliceKeys = step4["keys"]
    print(step4["message"])
    step5 = sm_py.step_5(step4["message"], bobKeys)
    bobKeys = step5["keys"]
    print(step5["success"])
    print(step5["message"])
    step6 = sm_py.step_6(step5["message"], aliceKeys)
    print(step6["success"])

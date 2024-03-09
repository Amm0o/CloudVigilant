# Onboarding Script for Cloud vigilante
import uuid
import json
import os

onboardingPath = "/opt/cloud-vigilante/"
onboardingJsonName = "cloudVigilanteOnboarding.json"

deviceId = uuid.uuid4()
tenantId = uuid.uuid4()


onboardingInfo = {
    "TenantID": str(deviceId),
    "DeviceID": str(tenantId)
}


os.makedirs(onboardingPath, exist_ok=True)

fullPath = os.path.join(onboardingPath, onboardingJsonName)

with open(fullPath, 'w') as json_file:
    json.dump(onboardingInfo, json_file, indent=4)


print(f"Onboarding Script has been written to: {fullPath}")


import smtplib
from email.mime.text import MIMEText
from geopy.distance import geodesic

# ---------- setting for send and recieve ----------
SENDER_EMAIL = "10422030@student.vgu.edu.vn"
APP_PASSWORD = ""
RECEIVER_EMAIL = "10422030@student.vgu.edu.vn"  # reciever

# ---------- function to send email ----------
def send_email(subject, body):
    msg = MIMEText(body)
    msg["Subject"] = subject
    msg["From"] = SENDER_EMAIL
    msg["To"] = RECEIVER_EMAIL

    with smtplib.SMTP_SSL("smtp.gmail.com", 465) as server:
        server.login(SENDER_EMAIL, APP_PASSWORD)
        server.send_message(msg)

    print("Email is sent!")


# ---------- Function to check coordinates in geofence area ----------
def is_inside_geofence(lat, lon, center_lat, center_lon, radius_m):
    device_location = (lat, lon)
    center_location = (center_lat, center_lon)
    distance = geodesic(device_location, center_location).meters
    return distance <= radius_m


# ---------- Main: Simulate a test ----------
if __name__ == "__main__":
    # Current coordinates of the device (simulated)
    current_lat = 10.762622
    current_lon = 106.660172

    # Geofence Zone Center
    geofence_lat = 10.762622
    geofence_lon = 106.682132
    radius = 500  # meters

    if is_inside_geofence(current_lat, current_lon, geofence_lat, geofence_lon, radius):
        send_email("Geofence Notification", " Device is IN geofence zone.")
    else:
        send_email("Geofence WARNING", "Device has left the geofence area!")

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <tf/tf.h>
#include <geometry_msgs/Twist.h>

ros::Publisher vel_pub;

void IMUCallback(sensor_msgs::Imu msg){
    if(msg.orientation_covariance[0] < 0)
        return;

    tf::Quaternion quaternion(
        msg.orientation.x,
        msg.orientation.y,
        msg.orientation.z,
        msg.orientation.w
    );

    double roll, pitch, yaw;
    tf::Matrix3x3(quaternion).getRPY(roll, pitch, yaw);
    roll = roll * 180 / M_PI;
    pitch = pitch * 180 / M_PI;
    yaw = yaw * 180 / M_PI;
    ROS_INFO("ROLL = %.0f  PITCH = %.0f  YAW = %.0f", roll, pitch, yaw);

    double target_yaw = 90;
    double diff_angel = target_yaw - yaw;
    geometry_msgs::Twist vel_cmd;
    vel_cmd.angular.z = diff_angel * 0.01;
    vel_pub.publish(vel_cmd);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_node");
    ros::NodeHandle n;
    ros::Subscriber imu_sub = n.subscribe<sensor_msgs::Imu>("/imu/data", 10, &IMUCallback);

    vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::spin();

    return 0;
}

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>

ros::Publisher vel_pub;
static int count = 0;

void LidarCallback(const sensor_msgs::LaserScan msg){
    float fMidDist = msg.ranges[180];
    ROS_INFO("The distance at ranges[180] = %f meters", fMidDist);
    geometry_msgs::Twist vel_cmd;

    if(count >0){
        count--;
        return;
    }

    if(fMidDist < 1.5){
        vel_cmd.angular.z = 0.3;
        count = 50;
    }else{
        vel_cmd.linear.x = 0.05;
    }
    vel_pub.publish(vel_cmd);
}

int main(int argc, char **argv){
    ros::init(argc, argv, "lidar_node");

    ros::NodeHandle n;
    ros::Subscriber lidar_sub = n.subscribe("/scan", 10, &LidarCallback);
    vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::spin();

    return 0;
}

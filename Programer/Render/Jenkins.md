1. 添加Jenkins的源（repository）
	sudo wget -O /etc/yum.repos.d/jenkins.repo http://jenkins-ci.org/redhat/jenkins.repo
	sudo rpm --import http://pkg.jenkins-ci.org/redhat/jenkins-ci.org.key
2. 安装/检查JDK
	java --version # 这个必须安装成功
	sudo yum install java-1.6.0-openjdk # 我用的1.8
3. 安装Jenkins:
	sudo yum install jenkins

安装完成后，有如下相关目录：
/usr/lib/jenkins/ :jenkins安装目录，WAR包会放在这里。
	cd /usr/lib/jenkins
	ll
4. 启动Jenkins
	echo &JAVA_HOME
	/usr/java/...(懒的复制了)
昨天还好好的，今天不知道为什么访问不了，各种查最后通过
	service jenkins start
搞定了，但是昨天一直没关，ps -aux | grep "jenkins" 也能找到。奇怪。
不过使用service jenkins start之后多了一个。
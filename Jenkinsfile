/*
 Significant references:
 * https://jenkins.io/doc/book/pipeline
 * https://github.com/adamrehn/ue4-opencv-demo/blob/master/project/OpenCVDemo/Jenkinsfile
 * https://github.com/adamrehn/jenkins-pipeline-utils/blob/master/vars/windowsContainer.groovy
*/

pipeline {
	agent none
	stages {
		stage('Build') {
			parallel {
				stage('Windows Build') {
					agent { label 'windows' }
					steps {
						// https://jenkins.io/blog/2017/07/26/powershell-pipeline/
						powershell(label:'Check Docker Version', script: 'docker version')
					}
				}
			}
		}
	}
}

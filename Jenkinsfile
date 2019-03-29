/*
 Significant references:
 * https://jenkins.io/doc/book/pipeline
 * https://github.com/adamrehn/ue4-opencv-demo/blob/master/project/OpenCVDemo/Jenkinsfile
 * https://github.com/adamrehn/jenkins-pipeline-utils/blob/master/vars/windowsContainer.groovy
*/

//Load the helper code for running Windows containers, since Jenkins does not yet feature native support
library identifier: 'jenkins-pipeline-utils@master', retriever: modernSCM([$class: 'GitSCMSource', remote: 'https://github.com/adamrehn/jenkins-pipeline-utils.git'])


pipeline {
	agent none
	stages {
		stage('Build') {
			parallel {
				stage('Windows Build') {
					agent { label 'windows && docker' }
					steps {
						// https://jenkins.io/blog/2017/07/26/powershell-pipeline/
						powershell(label:'Check Docker Version', script: 'docker version')
						windowsContainer('703768941458.dkr.ecr.us-east-1.amazonaws.com/ue4-docker/ue4-full:4.21.2', '-m 8GB',
						[
							'ue4 uat BuildCookRun -noP4 -clientconfig=Shipping -serverconfig=Shipping -cook -allmaps -build -stage -prereqs -pak -archive -archivedirectory="%cd%\\dist"',
							'7z a StealthGame.Win64.7z .\\dist'
						])
					}
					post
					{
						success { archiveArtifacts artifacts: 'StealthGame.Win64.7z' }
						cleanup { cleanWS() }
					}
				}
			}
		}
	}
}

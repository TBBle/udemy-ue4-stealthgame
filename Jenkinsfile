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
	environment {
		AWS_REGION='us-east-1'
		UE_VERSION='4.21.2'
		DOCKER_UE4FULL="703768941458.dkr.ecr.${env.AWS_REGION}.amazonaws.com/ue4-docker/ue4-full:${env.UE_VERSION}"
	}
	options { timestamps }
	stages {
		stage('Build') {
			parallel {
				stage('Windows Build') {
					agent { label 'windows && docker' }
					stages {
						stage('Prepare Docker Image') {
							steps {
								// https://jenkins.io/blog/2017/07/26/powershell-pipeline/
								powershell(label:'Check Docker Version', script: 'docker version')
								// https://aws.amazon.com/blogs/developer/new-get-ecrlogincommand-for-aws-tools-for-powershell/
								powershell(label:'Authenticate to ECR', script: "Invoke-Expression -Command (Get-ECRLoginCommand -Region ${env.AWS_REGION}).Command")
								powershell(label:'Pull ue4-full', script: "docker pull ${env.DOCKER_UE4FULL}")
							}
						}
						stage('Package - Shipping') {
							steps {
								windowsContainer("${env.DOCKER_UE4FULL}", '-m 8GB',
								[
									// https://adamrehn.com/docs/ue4cli/descriptor-commands/package
									'ue4 package Shipping',
									'7z a StealthGame.Win64.7z .\\dist'
								])
							}
							post {
								success { archiveArtifacts artifacts: 'StealthGame.Win64.7z' }
							}
						}
					}
					post {
						cleanup { cleanWS() }
					}
				}
			}
		}
	}
}

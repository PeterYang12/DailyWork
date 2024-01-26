#!/bin/bash

# This script deploy on your local host
if [[ $1 = "--help" ]] || [[ $1 = "-h" ]]
then
    echo "DEPLOY K8S ON YOUR LOCAL HOST"
    echo "USAGE:"
    echo "   ./single-node-setup.sh"
    exit 0
fi

#------------------------------------Install and Configure Container Runtime--------------------------------------------------------------
function docker_setup(){
    sudo apt-get update
    sudo apt-get install ca-certificates curl gnupg lsb-release
    sudo mkdir -m 0755 -p /etc/apt/keyrings
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
    echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

    sudo apt-get update
    sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

    # Add the docker daemon configurations to use systemd as the cgroup driver. 
    cat <<EOF | sudo tee /etc/docker/daemon.json
    {
      "exec-opts": ["native.cgroupdriver=systemd"], 
      "log-driver": "json-file",
      "log-opts": {
      "max-size": "100m" 
      },
      "storage-driver": "overlay2" 
    }
EOF
    # Start and enable the docker service. 
    sudo systemctl enable docker
    sudo systemctl daemon-reload
    sudo systemctl restart docker
}

#------------------------------------Install Kubeadm & Kubelet & Kubectl-----------------------------------------------------
function k8s_pre_install(){
    sudo apt-get update
    sudo apt-get install -y apt-transport-https ca-certificates curl
    sudo curl -fsSLo /usr/share/keyrings/kubernetes-archive-keyring.gpg https://packages.cloud.google.com/apt/doc/apt-key.gpg

    echo "deb [signed-by=/usr/share/keyrings/kubernetes-archive-keyring.gpg] https://apt.kubernetes.io/ kubernetes-xenial main" | sudo tee /etc/apt/sources.list.d/kubernetes.list
    sudo apt-get update -y
    sudo apt-get install -y kubelet kubeadm kubectl

    # hold to the packages to prevent upgrades. 
    sudo apt-mark hold kubelet kubeadm kubectl
}

#-----------------------------------------K8s Master node init---------------------------------------------------------------
function k8s_init(){
    # Remove the old containerd configuration
    sudo rm -rf /etc/containerd/config.toml
    sudo systemctl restart containerd

    POD_CIDR="192.168.0.0/16"
    sudo swapoff -a
    sudo kubeadm init --pod-network-cidr=$POD_CIDR

    mkdir -p $HOME/.kube
    sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config 
    sudo chown $(id -u):$(id -g) $HOME/.kube/config

    # Install Calico Network Plugin for Pod Networking
    echo "Install Calico Network Plugin for Pod Networking"
    kubectl apply -f https://raw.githubusercontent.com/projectcalico/calico/v3.25.0/manifests/calico.yaml

    # Taint the master node:
    kubectl taint nodes --all node-role.kubernetes.io/master-
}


docker_setup
k8s_pre_install
k8s_init
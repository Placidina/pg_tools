# pgtools

PGTools is a application to solve problems for PostgreSQL.

## TL;DR;

```console
# Add pgtools repository
helm repo add pgtools https://placidina.github.io/pg_tools
# Deploy a Helm Release named "my-release" using the pgtools chart
helm install my-release pgtools/pgtools
```

## Introduction

This chart bootstraps a [PGTools](https://github.com/Placidina/pg_tools) deployment on a [Kubernetes](http://kubernetes.io) cluster using the [Helm](https://helm.sh) package manager.

## Installing the Chart

To install the [Chart](https://helm.sh/docs/intro/using_helm/#three-big-concepts) with the [Release](https://helm.sh/docs/intro/using_helm/#three-big-concepts) name `my-release`:

```console
helm repo add pgtools https://placidina.github.io/pg_tools
helm install my-release pgtools/pgtools
```

The command deploys pgtools on the Kubernetes cluster in the default configuration.
The [configuration](#configuration) section lists the parameters that can be configured during installation.

## Uninstalling the Chart

To uninstall/delete the `my-release` deployment:

```console
helm delete my-release
```

The command removes all the Kubernetes components associated with the chart and deletes the release.

## Configuration

The following table lists the configurable parameters of the pgtools chart and their default values.

Parameter | Description | Default
---|---|---
`image.repository` | Repository for container image | `placidina/pg-tools`
`image.tag` | Image tag | `2.0.1`
`image.pullPolicy` | Image pull policy | `IfNotPresent`
`image.pullSecrets` | Image pull secrets | `[]`
`replicaCount` | Number of replicas | `1`
`deploymentStrategy` | Deployment strategy | `{rollingUpdate:{maxSurge:0,maxUnavailable:100%}}`
`deploymentLabels` | Additional deployment labels | `{}`
`deploymentAnnotations` | Additional deployment annotations | `{}`
`podLabels` | Additional pod labels | `{}`
`podAnnotations` | Additional pod annotations | `{}`
`nodeSelector` | Node labels for pod assignment | `{}`
`tolerations` | Toleration labels for pod assignment | `[]`
`affinity` | Affinity settings for pod assignment | `{}`
`priorityClassName` | Name of Priority Class to assign pods | `nil`
`resources` | Managing Compute Resources for Containers | `{}`
`extraInitContainers` | Containers, which are run before the app containers are started | `[]`
`commandOverrides` | Define a Command and Arguments for a Container | `[]`
`extraEnvs` | Additional environment variables to set | `[]`
`extraVolumeMounts` | Additional volumeMounts to the controller main container | `[]`
`extraVolumes` | Additional volumes to the controller pod | `[]`
`lifecycle` | Container lifecycle hooks | `{}`
`readinessProbe` | Readiness Probe settings | `{}`
`livenessProbe` | Liveness Probe settings | `{}`
`terminationGracePeriodSeconds` | Pod termination grace period | `30`
`config` | Custom pgtools.conf to override the default ones in the configmap | `nil`
`serviceAccount.create` | Whether a new service account name that the agent will use should be created. | `true`
`serviceAccount.annotations` | Additional annotations for service account | `{}`
`serviceAccount.name` | Service account to be used. If not set and serviceAccount.create is true a name is generated using the fullname template. | `empty`

Specify each parameter using the `--set key=value[,key=value]` argument to `helm install`. For example,

```console
helm install my-release pgtools/pgtools \
  --set=service.enabled=true,resources.limits.cpu=200m
```

Alternatively, a YAML file that specifies the values for the above parameters can be provided while installing the chart. For example,

```console
helm install -f values.yaml my-release pgtools/pgtools
```

> **Tip**: You can use the default [values.yaml](values.yaml), which is used by default, as reference
